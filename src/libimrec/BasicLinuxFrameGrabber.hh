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

#ifndef BASICLINUXFRAMEGRABBER_HH
#define BASICLINUXFRAMEGRABBER_HH

#include "FrameGrabberError.hh"
#include "RgbColor.hh"

#include <linux/videodev.h>

namespace imrec
{

/*======================================================================*/
/*!
 *  \class BgrColor BasicLinuxFrameGrabber.hh
 *  \brief The BgrColor class implements a simple 24-bit colortype
 *         which is the format supplied by the video source
 *
 */
/*======================================================================*/


  class BgrColor
  {
    uchar bc, gc, rc;
  public:
/*======================================================================*/
/*! 
 *   methods for getting the color components 
 *
 *   \return the color-component in 8-bit accuracy
 */
/*======================================================================*/

    uchar blue()  const { return bc; }
    uchar green() const { return gc; }
    uchar red()   const { return rc; }
  };
  
/*======================================================================*/
/*!
 *  \class BasicLinuxFrameGrabber BasicLinuxFrameGrabber.hh
 *  \brief The BasicLinuxFrameGrabber class implements a
 *         linux-specific frame-grabber-class. 
 *
 *  This class should not be used directly. Based on the OS-System 
 *  a typedef BasicFrameGrabber is defined to be either 
 *  BasicLinuxFrameGrabber, BasicSGIFrameGrabber or BasicDummyFrameGrabber.
 */
/*======================================================================*/

  class BasicLinuxFrameGrabber
  {
  public:
    typedef BgrColor value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    
    /*======================================================================*/
    /*! 
     *   Constructor. Open connection to video source, default is /dev/video0.  
     *
     *   \exception FrameGrabberError 
     */
    /*======================================================================*/
    BasicLinuxFrameGrabber(const char* device="/dev/video0");
    
    /*======================================================================*/
    /*! 
     *   Destructor. Closes connection to default video source.  
     */
    /*======================================================================*/
    ~BasicLinuxFrameGrabber();

    /*======================================================================*/
    /*! 
     *   return the number of rows of the actual frame
     *
     *   \return number of rows
     */
    /*======================================================================*/
    uint nrows() const;
    
    /*======================================================================*/
    /*! 
     *   return the number of columns of the actual frame
     *
     *   \return number of columns
     */
    /*======================================================================*/
    uint ncols() const;
    

    /*======================================================================*/
    /*! 
     *   return a pointer to the first pixel of the actual frame  
     *
     *   \return pointer to first pixel
     */
    /*======================================================================*/
    const_iterator imageBegin() const;

    /*======================================================================*/
    /*! 
     *   return a pointer to one past the last pixel of the actual frame 
     *
     *   \return pointer to one past the last pixel
     */
    /*======================================================================*/
    const_iterator imageEnd() const;

    /*======================================================================*/
    /*! 
     *   Check, if the next Frame is available (that means, that the
     *   next call to updateImageData will not block)
     *
     *   \return true: there is a new Frame available
     */
    /*======================================================================*/
    bool isNextFrameAvailable();


    /*======================================================================*/
    /*! 
     *   set frame data pointers to the most recently captured frame 
     */
    /*======================================================================*/
    void updateImageData();

    void releaseImageData()
          {}
    
  protected:
    /*======================================================================*/
    /*! 
     *   start capture of next frame and start a new thread, that
     *   waits for sync (capture ready) 
     *
     */
    /*======================================================================*/
    void startCapture();
    

  private:
    int dev;
    char* map;
    int frame;
    struct video_mbuf video_buffer_info;
    struct video_mmap video_map_info;
        
    pid_t syncWaitingProcessId;

    iterator p_imageBegin;
    iterator p_imageEnd;
    
    uint p_ncols;
    uint p_nrows;
  };
 
#include "BasicLinuxFrameGrabber.icc"
 
}

#endif //BASICLINUXFRAMEGRABBER_HH



