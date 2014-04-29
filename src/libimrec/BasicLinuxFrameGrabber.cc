// This file is part of VisAmp.
//
// VisAmp is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// VisAmp is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with VisAmp.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "types.hh"
#include "BasicLinuxFrameGrabber.hh"
#include "Rgb32BitColor.hh"

imrec::BasicLinuxFrameGrabber::BasicLinuxFrameGrabber(const char device[])
        : syncWaitingProcessId( -1)
{
  dev=-1;
  int max_try=5;
  
  //try to open the video device
  while ((max_try) && (dev==-1)) 
  {
    dev=open(device, O_RDWR);
    if (dev == -1) 
    {
      if (!--max_try) 
      {
        std::cerr << "Can't open device " << device << std::endl;
	exit(1);
      }
      sleep(1);
    }
  }
 
  //get video capabilities
  struct video_capability vid_caps;
  if (ioctl (dev, VIDIOCGCAP, &vid_caps) == -1) 
  {
    std::cerr << "VIDIOCGCAP failed" << std::endl;
    exit(1);
  }
  std::cerr << "device information:" << std::endl;
  std::cerr << "-------------------" << std::endl;
  std::cerr << "device name: " << vid_caps.name << std::endl;
  std::cerr << "device type: " << vid_caps.type << std::endl; 
  std::cerr << "#channels:   " << vid_caps.channels << std::endl;
  std::cerr << "#audios:     " << vid_caps.audios << std::endl;
  std::cerr << "maxwidth:    " << vid_caps.maxwidth << std::endl;
  std::cerr << "maxheight:   " << vid_caps.maxheight << std::endl;
  std::cerr << "minwidth:    " << vid_caps.minwidth << std::endl;
  std::cerr << "minheight:   " << vid_caps.minheight << std::endl << std::endl;
  
  //check if device supports capturing
  if ((vid_caps.type & VID_TYPE_CAPTURE)==0)
  {
    std::cerr << "Video device does NOT support capturing." << std::endl;
    exit (1);
  }
  

  /*-----------------------------------------------------------------------
   *  Get actual picture settings
   *-----------------------------------------------------------------------*/
  struct video_picture vid_picture;
  
  if (ioctl (dev, VIDIOCGPICT, &vid_picture) == -1) 
  {
    std::cerr << "VIDIOCGPICT failed" << std::endl;
    exit(1);
  }
  
  std::cerr << "Video picture settings:\n";
  std::cerr << "-----------------------\n";
  std::cerr << "brightness:     " << vid_picture.brightness << std::endl;
  std::cerr << "hue:            " << vid_picture.hue << std::endl;
  std::cerr << "colour:         " << vid_picture.colour << std::endl;
  std::cerr << "contrast:       " << vid_picture.contrast << std::endl;
  std::cerr << "whiteness:      " << vid_picture.whiteness << std::endl; /* Black and white only */
  std::cerr << "depth:          " << vid_picture.depth << std::endl;	 /* Capture depth */
  std::cerr << "palette:        " << vid_picture.palette << std::endl; 	/* Palette in use */
  
  /*-----------------------------------------------------------------------
   *  get capture settings
   *-----------------------------------------------------------------------*/
#if 0
  struct video_capture vid_capture;
  if (ioctl (dev, VIDIOCGCAPTURE, &vid_capture) == -1) 
  {
    std::cerr << "VIDIOCGCAPTURE failed" << std::endl;
    exit(1);
  }
  std::cerr << "video capture settings:" << std::endl;
  std::cerr << "-------------------------" << std::endl;
  std::cerr << "x,y;		" << vid_capture.x << "," << vid_capture.y << std::endl;                 /* Offsets into image */
  std::cerr << "width, height;	" << vid_capture.width << "," << vid_capture.height << std::endl;         /* Area to capture */
  std::cerr << "decimation;	" << vid_capture.decimation << std::endl;            /* Decimation divder */
  std::cerr << "flags;		" << vid_capture.flags << std::endl;                 /* Flags for capture */
  
#endif
  //initialize structs

  p_ncols=vid_caps.maxwidth/2;
  p_nrows=vid_caps.maxheight/2;
  
  p_imageBegin = 0;
  p_imageEnd   = 0;
  
  video_map_info.format = VIDEO_PALETTE_RGB24;
  video_map_info.width  = p_ncols;
  video_map_info.height = p_nrows;  
  
  //try if channels can be changed, and change to default input channel
  struct video_channel vid_chnl;
  vid_chnl.channel = 0;
  if (ioctl (dev, VIDIOCGCHAN, &vid_chnl) == -1) 
  {
    std::cerr << "ioctl (VIDIOCGCHAN) failed" << std::endl;
  } 
  else 
  {
    vid_chnl.channel = 0;
    vid_chnl.norm    = VIDEO_MODE_PAL;
    if (ioctl (dev, VIDIOCSCHAN, &vid_chnl) == -1) 
    {
      std::cerr << "ioctl (VIDIOCSCHAN) failed" << std::endl;
      exit(1);
    }
  }

  //check if there is a video buffer
  if (ioctl (dev, VIDIOCGMBUF, &video_buffer_info) == -1)
  {
    std::cerr << "no video buffer available!" << std::endl;
    exit(1);
  }
  std::cerr << "video buffer information:" << std::endl;
  std::cerr << "-------------------------" << std::endl;
  std::cerr << "Total memory size: " << video_buffer_info.size << std::endl;
  std::cerr << "#Frames          : " << video_buffer_info.frames << std::endl;
  frame=0;

  //map buffer to video device buffer
  map=(char*)mmap(0, 
                  video_buffer_info.size, 
                  PROT_READ,
                  MAP_SHARED,dev,
                  0);
  if (map==(char*)-1) 
  {
    std::cerr << "mapping failed " << std::endl;
    exit(1);
  }

  /*-----------------------------------------------------------------------
   *  start grabbing of first image
   *-----------------------------------------------------------------------*/
  startCapture();
  
}

//-----------------------------------------------------------------------

imrec::BasicLinuxFrameGrabber::~BasicLinuxFrameGrabber()
{
  //release buffer mapping
  munmap(map, video_buffer_info.size);
  //close device
  if (dev>=0) close(dev);
}


//-----------------------------------------------------------------------

bool imrec::BasicLinuxFrameGrabber::isNextFrameAvailable()
{
  if( syncWaitingProcessId != -1)
  {
    int   status = 0;
    pid_t returnedPid;
    errno = 0;
    returnedPid = waitpid( syncWaitingProcessId, &status, WNOHANG);
    if( returnedPid == 0)
    {
      /*-------------------------------------------------------------------
       *  child is still running and therefore waiting for sync
       *-------------------------------------------------------------------*/
      return false;
    }
  }

  /*---------------------------------------------------------------------
   *  child just exited or is already dead
   *---------------------------------------------------------------------*/
  return true;
}
  


//-----------------------------------------------------------------------

void imrec::BasicLinuxFrameGrabber::updateImageData()
{ 
  /*-----------------------------------------------------------------------
   *  wait until actual Frame is available
   *-----------------------------------------------------------------------*/
  int   status = 0;
  waitpid( syncWaitingProcessId, &status, 0);

  /*-----------------------------------------------------------------------
   *  set begin and end to grabbed frame
   *-----------------------------------------------------------------------*/
  p_imageBegin = reinterpret_cast<value_type*>
      (map + video_buffer_info.offsets[frame]);
  p_imageEnd = p_imageBegin + p_ncols*p_nrows;


  /*-----------------------------------------------------------------------
   *  Start grabbing into next framebuffer
   *-----------------------------------------------------------------------*/
  startCapture();
 
}

//-----------------------------------------------------------------------

void imrec::BasicLinuxFrameGrabber::startCapture()
{
  /*-----------------------------------------------------------------------
   *  Start capture into next framebuffer
   *-----------------------------------------------------------------------*/
  frame=(frame+1) % video_buffer_info.frames;
  video_map_info.frame  = frame;

  if (ioctl (dev, VIDIOCMCAPTURE, &video_map_info) == -1) 
  {
    std::cerr << "VIDIOCMCAPTURE failed" << std::endl;
    exit(1);
  } 
  
  /*-----------------------------------------------------------------------
   *  start thread, that waits for sync
   *-----------------------------------------------------------------------*/
  syncWaitingProcessId = fork();
  if( syncWaitingProcessId == -1)
  {
    std::cerr << "fork() failed!" << std::endl;
    exit(-1);
  }

  if (syncWaitingProcessId == 0) 
  {
    /*-------------------------------------------------------------------
     *  This is the child process. Wait for sync and die
     *-------------------------------------------------------------------*/
    video_map_info.frame  = frame;
    if (ioctl (dev, VIDIOCSYNC, &video_map_info) == -1) 
    {
      std::cerr << "VIDIOCSYNC failed" << std::endl;
      exit(1);
    }
    exit( 42);
  }
  
  /*-----------------------------------------------------------------------
   *  parent process just returns
   *-----------------------------------------------------------------------*/
  return;
}

    
