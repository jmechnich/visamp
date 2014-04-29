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
#include <dmedia/vl.h>

#include "BasicSGIFrameGrabber.hh"

//! \todo change printf -> cout

/*======================================================================*/
/*! 
 *   debugging function to get  the plain text packing name
 *
 */
/*======================================================================*/
static char *packing_name (int packtype)
{
    switch (packtype) 
    {
	case VL_PACKING_RGB_8:    
	    return "RGB";
	break;
	
        case VL_PACKING_RGBA_8:   
	    return "RGBA";
	break;
	
	case VL_PACKING_RBG_323:   
	    return "Starter Video RGB8";
	break;
	
	case VL_PACKING_RGB_332_P:	
	    return "RGB332";
	break;
	
	case VL_PACKING_Y_8_P: 
	    return "8 Bit Greyscale";
	break;
	
	default:		
	    return "???";
	break;
    }
}


//-----------------------------------------------------------------------

imrec::BasicSGIFrameGrabber::BasicSGIFrameGrabber(const char* device)
{
  imrec::BasicSGIFrameGrabber();
}


//-----------------------------------------------------------------------

imrec::BasicSGIFrameGrabber::BasicSGIFrameGrabber()
        : vidsrc( VL_ANY),
          zoom_num(1),
          zoom_denom(2),
          rate(0),
          nbuffers(2)
{
  /* Connect to the video daemon */
  if (!(vlSvr = vlOpenVideo(""))) 
  {
    printf("couldn't open video\n");
    exit(1);
  }
  
  /* Get the list of the devices that the daemon supports */
  VLDevList devlist;
  if (vlGetDeviceList(vlSvr, &devlist) < 0) 
  {
    printf("error getting device list: %s\n",vlStrError(vlErrno));
    exit(1);
  }

  /* Set up a source node on the specified video source */
  printf("using video source number: %d\n", vidsrc);
  VLNode src = vlGetNode(vlSvr, VL_SRC, VL_VIDEO, vidsrc);

  /* Set up a drain node in memory */
  VLNode drn = vlGetNode(vlSvr, VL_DRN, VL_MEM, VL_ANY);
  
  /* 
   * Create a path from video to memory. 
   */
  int i;
  
  VLPath vlPath = vlCreatePath(vlSvr, VL_ANY, src, drn);
  if (vlPath < 0) 
  {
    printf("can't create path: %s\n", vlStrError(vlErrno));
    exit(1);
  }
  /* Get the device number for future use */
  VLDev deviceId = vlGetDevice(vlSvr, vlPath); 
  int devicenum = -1;
  for (i=0; i<devlist.numDevices; i++)
  {
    if (devlist.devices[i].dev == deviceId)
    {
      devicenum = i;
      break;
    }
  } 

  /* Set up the hardware for and define the usage of the path */
  if (vlSetupPaths(vlSvr, (VLPathList)&vlPath, 1, VL_SHARE, VL_SHARE) < 0)
  {
    printf ("could not setup a vid to mem path\n");
    exit(1);
  }

  /* Get the name of the device we're using */
  char* deviceName = devlist.devices[devicenum].name;
  printf( "using device `%s'\n", deviceName);
  
  
  /* Set the zoom ratio */
  VLControlValue val;
  val.fractVal.numerator = zoom_num;
  val.fractVal.denominator = zoom_denom;
  vlSetControl(vlSvr, vlPath, drn, VL_ZOOM, &val);
    
  /* Set the frame rate */
  if (rate) 
  {
    val.fractVal.numerator = rate;
    val.fractVal.denominator = 1;
    vlSetControl(vlSvr, vlPath, drn, VL_RATE, &val);
  }
  vlGetControl(vlSvr, vlPath, drn, VL_RATE, &val);
  printf("frame rate = %d/%d\n", val.fractVal.numerator,
        val.fractVal.denominator);

  /* 
   * Specify what vlPath-related events we want to receive.
   * In this example we only want transfer complete, transfer
   * failed and stream preempted events.
   */
  vlSelectEvents(vlSvr, vlPath, VLTransferCompleteMask|
                 VLStreamPreemptedMask|VLTransferFailedMask);

  if (vlGetControl(vlSvr, vlPath, drn, VL_CAP_TYPE, &val) < 0)
      vlPerror("Getting VL_CAP_TYPE");
  printf("cap type = %d\n", val.intVal);

  /* Make sure this size is supported by the hardware */
  vlGetControl(vlSvr, vlPath, drn, VL_SIZE, &val);
  p_ncols = val.xyVal.x;
  p_nrows = val.xyVal.y;
  printf("grabbing size %dx%d\n", val.xyVal.x, val.xyVal.y);
  
  /* Set the graphics to the same packing as the video board */
  vlGetControl(vlSvr, vlPath, drn, VL_PACKING, &val);
  packing = val.intVal = VL_PACKING_RGB_8;
  if (vlSetControl(vlSvr, vlPath, drn, VL_PACKING, &val) < 0)
      vlPerror("Setting RGB");
  vlGetControl(vlSvr, vlPath, drn, VL_PACKING, &val);
  printf("packing type now is %s\n", packing_name(val.intVal));
  if (val.intVal != packing)
  {
    fprintf(stderr, "Error, could not set Packing to %s",
            packing_name (packing));
    exit(1);
  }

  /* Set up the ring buffer for data transfer */
  transferBuf = vlCreateBuffer(vlSvr, vlPath, drn, nbuffers);
  
  /* Associate the ring buffer with the path */
  vlRegisterBuffer(vlSvr, vlPath, drn, transferBuf);

  /* Begin the data transfer */
  if (vlBeginTransfer(vlSvr, vlPath, 0, NULL) < 0)
  {
    vlPerror("vlBeginTransfer");
    exit(1);
  }
}

//-----------------------------------------------------------------------

void imrec::BasicSGIFrameGrabber::updateImageData()
{
  /*-----------------------------------------------------------------------
   *  We are done with the last transferbuffer
   *-----------------------------------------------------------------------*/
  vlPutFree(vlSvr, transferBuf);


  /*-----------------------------------------------------------------------
   *  Wait for frame to become available
   *  Get a pointer to the most recently captured frame
   *-----------------------------------------------------------------------*/
  VLInfoPtr info;
  while ( (info = vlGetLatestValid(vlSvr, transferBuf)) == 0)
  {
//    cout << "waiting for frame...\n";
  }

  /* Get the valid video data from that frame */
  void* dataPtr = vlGetActiveRegion(vlSvr, transferBuf, info);

  /*-----------------------------------------------------------------------
   *  Set begin and end pointer appropriately
   *-----------------------------------------------------------------------*/
  p_imageBegin = reinterpret_cast<Rgb32BitColor*>( dataPtr);
  p_imageEnd = p_imageBegin + nrows() * ncols();
}

