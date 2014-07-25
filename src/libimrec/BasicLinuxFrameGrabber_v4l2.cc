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
#include <cstdio>
#include <cstdlib>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iomanip>
#include <libv4l2.h>

#include "types.hh"
#include "BasicLinuxFrameGrabber_v4l2.hh"
#include "Rgb32BitColor.hh"

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
imrec::BasicLinuxFrameGrabber::BasicLinuxFrameGrabber(const char device[])
        : dev( -1)
        , map( 0)
        , frame( -1)
        , syncWaitingProcessId( -1)
        , p_imageBegin( 0), p_imageEnd( 0)
        , p_pixformat( V4L2_PIX_FMT_BGR24), p_ncols( 0), p_nrows( 0), p_fps( 30)
        , p_buffers( 0), p_nBuffers( 2)
{
  p_ncols = 320;
  p_nrows = 240;
  
  clearPBuf();
  deviceOpen(device);
  deviceInfo();
  deviceInit();
  mmapInit();
  
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (-1 == xioctl(dev, VIDIOC_STREAMON, &type))
      errno_exit("VIDIOC_STREAMON");
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
imrec::BasicLinuxFrameGrabber::~BasicLinuxFrameGrabber()
{
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if( -1 == xioctl( dev, VIDIOC_STREAMOFF, &type))
      errno_exit("VIDIOC_STREAMOFF");
  
  //release buffer mapping
  for( size_t i=0; i < p_nBuffers; ++i)
      v4l2_munmap( p_buffers[i].start, p_buffers[i].length);
  delete[] p_buffers;
  
  //close device
  if (dev>=0) v4l2_close(dev);
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void imrec::BasicLinuxFrameGrabber::deviceOpen( const char* device)
{
  int max_try=5;
  //try to open the video device
  while ((max_try) && (dev==-1)) 
  {
    dev=v4l2_open(device, O_RDWR, 0);
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
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void imrec::BasicLinuxFrameGrabber::clearPBuf()
{
  memset( &p_buf, 0, sizeof(p_buf));
  p_buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  p_buf.memory = V4L2_MEMORY_MMAP;
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void imrec::BasicLinuxFrameGrabber::printPBuf()
{
  std::cerr << "buffer info:" << std::endl
            << "------------" << std::endl
            << "index:     " << p_buf.index << std::endl
            << "type:      " << p_buf.type << std::endl
            << "bytesused: " << p_buf.bytesused << std::endl
            << "flags:     " << std::hex << p_buf.flags << std::dec << std::endl
            << "timestamp: " << p_buf.timestamp.tv_sec << "."
            << p_buf.timestamp.tv_usec << std::endl
            << "sequence:  " << p_buf.sequence << std::endl
            << "offset:    " << std::hex << p_buf.m.offset << std::dec << std::endl
            << "length:    " << p_buf.length << std::endl
            << std::endl;
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void imrec::BasicLinuxFrameGrabber::deviceInfo()
{
  /*-----------------------------------------------------------------------
   *  Get video capabilities
   *-----------------------------------------------------------------------*/
  v4l2_capability vid_caps = {0};
  
  if (ioctl (dev, VIDIOC_QUERYCAP, &vid_caps) == -1) 
  {
    std::cerr << "VIDIOC_QUERYCAP failed" << std::endl;
    exit(1);
  }
  std::cerr << "device information:" << std::endl;
  std::cerr << "-------------------" << std::endl;
  std::cerr << "driver:  " << vid_caps.driver << std::endl;
  std::cerr << "card:    " << vid_caps.card << std::endl; 
  std::cerr << "bus:     " << vid_caps.bus_info << std::endl;
  std::cerr << "version: " << ((vid_caps.version>>16)&&0xff) << "."
            << ((vid_caps.version>>24)&&0xff) << std::endl;
  std::cerr << "caps:    " << std::hex << vid_caps.capabilities << std::dec << std::endl << std::endl;
  
  //check if device supports capturing
  if ((vid_caps.capabilities & V4L2_CAP_VIDEO_CAPTURE)==0)
  {
    std::cerr << "Video device does NOT support capturing." << std::endl;
    exit(1);
  }
  
  if (!(vid_caps.capabilities & V4L2_CAP_STREAMING))
  {
    std::cerr << "Video device does NOT support streaming i/o" << std::endl;
    exit(1);
  }
  
  
  /*-----------------------------------------------------------------------
   *  Query cropping capabilities
   *-----------------------------------------------------------------------*/
  struct v4l2_cropcap cropcap = {0};
  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if( 0 == xioctl( dev, VIDIOC_CROPCAP, &cropcap))
  {
    std::cerr << "cropping:" << std::endl;
    std::cerr << "-------------------" << std::endl;
    fprintf( stderr, "Bounds: %dx%d+%d+%d\n",
            cropcap.bounds.width, cropcap.bounds.height,
            cropcap.bounds.left, cropcap.bounds.top);
    fprintf( stderr, "Default: %dx%d+%d+%d\n",
            cropcap.defrect.width, cropcap.defrect.height,
            cropcap.defrect.left, cropcap.defrect.top);
    fprintf( stderr, "Pixel aspect: %d/%d\n\n",
            cropcap.pixelaspect.numerator,
            cropcap.pixelaspect.denominator);

    struct v4l2_crop crop={0};
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect; /* reset to default */
    
    if (-1 == xioctl(dev, VIDIOC_S_CROP, &crop))
    {
      switch (errno)
      {
      case EINVAL:
        /* Cropping not supported. */
        break;
      default:
        /* Errors ignored. */
        break;
      }
    }
  }
  /*-----------------------------------------------------------------------
   *  Query supported image formats
   *-----------------------------------------------------------------------*/
  bool pixformat_supported = false;
  
  struct v4l2_fmtdesc fmtdesc = {0};
  struct v4l2_frmsizeenum frmsize = {0};
  struct v4l2_frmivalenum frmival = {0};
 
  fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  char fourcc[5] = {0};
  char sel, c, e, t;
  std::cerr << "image formats:" << std::endl;
  std::cerr << "-------------------" << std::endl;
  fprintf( stderr," FMT : CE Desc\n--------------------\n");
  int ret =0;
  while (0 == xioctl( dev, VIDIOC_ENUM_FMT, &fmtdesc))
  {
    if( fmtdesc.pixelformat == p_pixformat)
        pixformat_supported = true;
    sel = fmtdesc.pixelformat == p_pixformat ? '*' : ' ';
    strncpy(fourcc, (char *)&fmtdesc.pixelformat, 4);
    c = fmtdesc.flags & 1? 'C' : ' ';
    e = fmtdesc.flags & 2? 'E' : ' ';
    fprintf( stderr,"%c%s: %c%c %s\n", sel, fourcc, c, e, fmtdesc.description);
    frmsize.pixel_format = fmtdesc.pixelformat;
    frmsize.index = 0;
    fprintf( stderr,"         ");
    while( xioctl( dev, VIDIOC_ENUM_FRAMESIZES, &frmsize) == 0)
    {
      t = ' ';
      if( frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE)
      {
        // if( p_ncols < frmsize.discrete.width || 
        //     p_nrows < frmsize.discrete.height)
        if( p_ncols == 0)
        {
          p_ncols = frmsize.discrete.width;
          p_nrows = frmsize.discrete.height;
        }
        
        t = 'D';
        fprintf( stderr," %c%dx%d", t,
               frmsize.discrete.width,
               frmsize.discrete.height);
      }
      else if( frmsize.type == V4L2_FRMSIZE_TYPE_STEPWISE)
      {
        t = 'S';
        fprintf( stderr," %c%dx%d", t,
               frmsize.stepwise.max_width,
               frmsize.stepwise.max_height);
      }
      frmsize.index++;
    }
    fprintf( stderr,"\n");
    fmtdesc.index++;
  }
  if( errno != EINVAL)
  {
    std::cerr << "Error enumerating formats" << std::endl;
    std::exit(0);
  }
  std::cerr << std::endl;
  
  /*-----------------------------------------------------------------------
   *  Control capabilities
   *-----------------------------------------------------------------------*/
  std::cerr << "control capabilities:" << std::endl;
  std::cerr << "-------------------" << std::endl;
  struct v4l2_queryctrl queryctrl={0};
  struct v4l2_control control={0};
  for (queryctrl.id = V4L2_CID_BASE;
       queryctrl.id < V4L2_CID_LASTP1;
       queryctrl.id++) {
    if (0 == xioctl (dev, VIDIOC_QUERYCTRL, &queryctrl))
    {
      if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) continue;
      control.id = queryctrl.id;
      if( -1 == xioctl (dev, VIDIOC_G_CTRL, &control))
      {
        perror ("VIDIOC_G_CTRL");
        exit (EXIT_FAILURE);
      }
      if( !(queryctrl.flags & V4L2_CTRL_FLAG_GRABBED   ||
            queryctrl.flags & V4L2_CTRL_FLAG_READ_ONLY ||
            queryctrl.flags & V4L2_CTRL_FLAG_INACTIVE) )
      {
        control.value = queryctrl.default_value;
        if( -1 == xioctl (dev, VIDIOC_S_CTRL, &control))
        {
          errno_exit( "VIDIOC_S_CTRL");
          perror ("VIDIOC_S_CTRL");
          exit (EXIT_FAILURE);
        }
      }
      
      fprintf( stderr, "Control %s: %d (range: %d - %d, default: %d)\n",
              queryctrl.name, control.value,
              queryctrl.minimum, queryctrl.maximum,
              queryctrl.default_value);
    }
    else
    {
      if (errno == EINVAL) continue;
      perror ("VIDIOC_QUERYCTRL");
      exit (EXIT_FAILURE);
    }
  }
  
  for( queryctrl.id = V4L2_CID_PRIVATE_BASE; ; queryctrl.id++)
  {
    if (0 == xioctl (dev, VIDIOC_QUERYCTRL, &queryctrl))
    {
      if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) continue;
      fprintf( stderr, "Control %s\n", queryctrl.name);
    }
    else
    {
      if(errno == EINVAL) break;
      perror ("VIDIOC_QUERYCTRL");
      exit (EXIT_FAILURE);
    }
  }
  std::cerr << std::endl;
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void imrec::BasicLinuxFrameGrabber::deviceInit()
{
  struct v4l2_format fmt={0};
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (-1 == xioctl(dev, VIDIOC_G_FMT, &fmt))
  {
    std::cerr << "VIDIOC_G_FMT failed" << std::endl;
    std::exit(1);
  }
  fmt.fmt.pix.width  = p_ncols;
  fmt.fmt.pix.height = p_nrows;
  fmt.fmt.pix.field  = V4L2_FIELD_INTERLACED;
  fmt.fmt.pix.pixelformat = p_pixformat;
    
  if (-1 == xioctl(dev, VIDIOC_S_FMT, &fmt))
  {
    std::cerr << "VIDIOC_S_FMT failed" << std::endl;
    std::exit(1);
  }
  
  std::cerr << "image format:\n" 
            << "-------------\n"
            << "width:        " << fmt.fmt.pix.width << std::endl
            << "height:       " << fmt.fmt.pix.height << std::endl
            << "pixelformat:  " << std::hex << "0x" << fmt.fmt.pix.pixelformat
            << std::dec << std::endl
            << "field:        " << std::hex << "0x" << fmt.fmt.pix.field << std::dec << std::endl
            << "bytesperline: " << fmt.fmt.pix.bytesperline << std::endl
            << "sizeimage:    " << fmt.fmt.pix.sizeimage << std::endl
            << "colorspace:   " << std::hex << "0x" << fmt.fmt.pix.colorspace << std::dec << std::endl
            << std::endl;
  
  v4l2_frmivalenum frmival = {0};
  frmival.index        = 0;
  frmival.pixel_format = fmt.fmt.pix.pixelformat;
  frmival.width        = fmt.fmt.pix.width;
  frmival.height       = fmt.fmt.pix.height;
  if( 0 == xioctl(dev, VIDIOC_ENUM_FRAMEINTERVALS, &frmival))
  {
    std::cerr << "frame interval:\n"
              <<"----------------\n";
    if( frmival.type == V4L2_FRMIVAL_TYPE_DISCRETE)
    {
      std::cerr << "type: " << frmival.type << " (discrete)" << std::endl;
      while( true)
      {
        frmival.index+=1;
        if( -1 == xioctl(dev, VIDIOC_ENUM_FRAMEINTERVALS, &frmival))
        {
          if( errno == EINVAL)
              break;
          std::cerr << "VIDIOC_ENUM_FRAMEINTERVALS discrete" << std::endl;
          std::exit(1);
        }
        fprintf( stderr, " %d/%d", frmival.discrete.numerator, frmival.discrete.denominator);
      }
      std::cerr << std::endl;
    }
    else
    {
      if( frmival.type == V4L2_FRMIVAL_TYPE_CONTINUOUS)
      {
        std::cerr << "type: " << frmival.type << " (continuous)" << std::endl;
      }
      else if( frmival.type == V4L2_FRMIVAL_TYPE_STEPWISE)
      {
        std::cerr << "type: " << frmival.type << " (stepwise)" << std::endl;
      }
      fprintf( stderr, "min:  %d/%d\n",
               frmival.stepwise.min.numerator, frmival.stepwise.min.denominator);
      fprintf( stderr, "max:  %d/%d\n",
               frmival.stepwise.max.numerator, frmival.stepwise.max.denominator);
      fprintf( stderr, "step: %d/%d\n",
               frmival.stepwise.step.numerator, frmival.stepwise.step.denominator);
    }
    std::cerr << std::endl;
  }
  else
  {
    switch( errno)
    {
    case EINVAL:
      break;
    default:
      errno_exit( "VIDIOC_ENUM_FRAMEINTERVALS");
      break;
    }
  }
  
  struct v4l2_streamparm frameint={0};
  frameint.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if( frameint.parm.capture.capability & V4L2_CAP_TIMEPERFRAME)
  {
    std::cerr << "Framerate set to " << p_fps << " seconds." << std::endl;
    frameint.parm.capture.timeperframe.numerator = 1;
    frameint.parm.capture.timeperframe.denominator = p_fps;
  }
  if (-1 == xioctl(dev, VIDIOC_S_PARM, &frameint))
      fprintf(stderr,"Unable to set frame interval.\n");
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void imrec::BasicLinuxFrameGrabber::mmapInit()
{
  struct v4l2_requestbuffers req={0};
  req.count = p_nBuffers;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;

  if( -1 == xioctl(dev, VIDIOC_REQBUFS, &req))
  {
    if (EINVAL == errno)
    {
      std::cerr <<  "Device does not support memory mapping" << std::endl;
      exit(EXIT_FAILURE);
    }
    else
        errno_exit("VIDIOC_REQBUFS");
  }
  
  if (req.count < p_nBuffers)
  {
    std:: cerr << "Insufficient buffer memory" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  p_buffers = new Buffer[p_nBuffers];
  
  if( !p_buffers)
  {
    std::cerr << "Out of memory" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  for( size_t i=0; i < p_nBuffers; ++i)
  {
    clearPBuf();
    p_buf.index  = i;
    
    if( -1 == xioctl(dev, VIDIOC_QUERYBUF, &p_buf))
        errno_exit("VIDIOC_QUERYBUF");
    
    p_buffers[i].length = p_buf.length;
    p_buffers[i].start = v4l2_mmap(
        0, p_buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, dev, p_buf.m.offset);
    
    if (MAP_FAILED == p_buffers[i].start)
        errno_exit("mmap");
   }
  for( size_t i=0; i < p_nBuffers; ++i)
  {
    clearPBuf();
    p_buf.index  = i;
    if (-1 == xioctl(dev, VIDIOC_QBUF, &p_buf))
        errno_exit("VIDIOC_QBUF");
  }
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
bool imrec::BasicLinuxFrameGrabber::isNextFrameAvailable()
{
  if( frame != -1) return false;
  
  for( size_t i=0; i< p_nBuffers; ++i)
  {
    clearPBuf();
    p_buf.index = i;
    if( -1 == xioctl(dev, VIDIOC_QUERYBUF, &p_buf))
        errno_exit("VIDIOC_QUERYBUF");
    if( p_buf.flags & V4L2_BUF_FLAG_DONE)
    {
      return true;
    }
  }
  return false;
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void imrec::BasicLinuxFrameGrabber::updateImageData()
{ 
  /*-----------------------------------------------------------------------
   *  wait until actual Frame is available
   *-----------------------------------------------------------------------*/
  clearPBuf();
  if (-1 == xioctl(dev, VIDIOC_DQBUF, &p_buf))
      errno_exit("VIDIOC_DQBUF");
  frame = p_buf.index;
  
  /*-----------------------------------------------------------------------
   *  set begin and end to grabbed frame
   *-----------------------------------------------------------------------*/
  p_imageBegin = reinterpret_cast<value_type*>(p_buffers[frame].start);
  p_imageEnd   = p_imageBegin + p_ncols*p_nrows;
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void imrec::BasicLinuxFrameGrabber::releaseImageData()
{
  if( frame == -1) return;
  
  clearPBuf();
  p_buf.index = frame;
  if (-1 == xioctl(dev, VIDIOC_QBUF, &p_buf))
      errno_exit("VIDIOC_QBUF");
  frame = -1;
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void imrec::BasicLinuxFrameGrabber::startCapture()
{
  return;
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
int imrec::BasicLinuxFrameGrabber::xioctl(int fd, int request, void *arg)
{
  int r;
  
  do r = v4l2_ioctl (fd, request, arg);
  while (-1 == r && EINTR == errno);
  
  return r;
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void imrec::BasicLinuxFrameGrabber::errno_exit(const char* s)
{
  fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
  exit(EXIT_FAILURE);
}
