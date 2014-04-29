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

#ifndef BASICSGIFRAMEGRABBER_HH
#define BASICSGIFRAMEGRABBER_HH

#include <dmedia/vl.h>
#include "FrameGrabberError.hh"
#include "Rgb32BitColor.hh"

namespace imrec
{

/*======================================================================*/
/*!
 *  \class BasicSGIFrameGrabber BasicSGIFrameGrabber.hh
 *  \brief The BasicSGIFrameGrabber class implements a
 *         SGI-specific frame-grabber-class. 
 *
 *  This class should not be used directly. Based on the OS-System 
 *  a typedef BasicFrameGrabber is defined to be either 
 *  BasicLinuxFrameGrabber, BasicSGIFrameGrabber or BasicDummyFrameGrabber.
 */
/*======================================================================*/

  class BasicSGIFrameGrabber
  {
  public:

    typedef Rgb32BitColor        value_type;
    typedef Rgb32BitColor*       iterator;
    typedef const Rgb32BitColor* const_iterator;
    
    /*======================================================================*/
    /*! 
     *   Constructor. Open connection to default video source
     *
     *   \exception FrameGrabberError
     */
    /*======================================================================*/
    BasicSGIFrameGrabber();
    BasicSGIFrameGrabber(const char* device); //dummy constructor only to be combatible 
                                        // with BasicLinuxGrabber

    
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
     *   Check, if the next Frame is available. On SGI this is a dummy
     *   function, as the built in frame grabber has a frame rate of 30
     *   images per second 
     *
     *   \return always true
     */
    /*======================================================================*/
    bool isNextFrameAvailable()
          { return true;}

    /*======================================================================*/
    /*! 
     *   set frame data pointers to the most recently captured frame 
     */
    /*======================================================================*/
    void updateImageData();

    void releaseImageData()
          {}
    
  private:
    int		   vidsrc;
    VLServer	   vlSvr;
    int            packing;
    int            zoom_num;         // zoom numerator and 
    int            zoom_denom;       // denominator   
    int            rate;             // frame rate
    int            nbuffers;         // number of buffers in ring buffer
    VLBuffer       transferBuf;
    Rgb32BitColor* p_imageBegin;
    Rgb32BitColor* p_imageEnd;
    uint           p_nrows;
    uint           p_ncols;
  };
  
#include "BasicSGIFrameGrabber.icc"
  
}



#endif //BASICSGIFRAMEGRABBER_HH
 
