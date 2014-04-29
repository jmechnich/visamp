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

#ifndef BASICDUMMYFRAMEGRABBER_HH
#define BASICDUMMYFRAMEGRABBER_HH

#include <iostream>

#include "Rgb32BitColor.hh"

namespace imrec
{
/*======================================================================*/
/*!
 *  \class BasicDummyFrameGrabber BasicDummyFrameGrabber.hh
 *  \brief The BasicDummyFrameGrabber class is used as framegrabber class
 *         on non-IRIX and non-Linux Systems. It only offers the interface, 
 *         no functionality. 
 *
 *  This class should not be used directly. Based on the OS-System 
 *  a typedef BasicFrameGrabber is defined to be either 
 *  BasicLinuxFrameGrabber, BasicSGIFrameGrabber or BasicDummyFrameGrabber.
 */
/*======================================================================*/

  class BasicDummyFrameGrabber
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
    BasicDummyFrameGrabber()
          {
            std::cerr << "Sorry, no framegrabber available!\n";
          }
    

    
    /*======================================================================*/
    /*! 
     *   return the number of rows of the actual frame
     *
     *   \return number of rows
     */
    /*======================================================================*/
    uint nrows() const
          {
            return 1;
          }
    
    
    /*======================================================================*/
    /*! 
     *   return the number of columns of the actual frame
     *
     *   \return number of columns
     */
    /*======================================================================*/
    uint ncols() const
          {
            return 1;
          }
   

    /*======================================================================*/
    /*! 
     *   return a pointer to the first pixel of the actual frame  
     *
     *   \return pointer to first pixel
     */
    /*======================================================================*/
    const_iterator imageBegin() const
          {
            return &onePixelImage;
          }
    

    /*======================================================================*/
    /*! 
     *   return a pointer to one past the last pixel of the actual frame 
     *
     *   \return pointer to one past the last pixel
     */
    /*======================================================================*/
    const_iterator imageEnd() const
          {
            return &onePixelImage + 1;
          }
    

    /*======================================================================*/
    /*! 
     *   set frame data pointers to the most recently captured frame 
     */
    /*======================================================================*/
    void updateImageData()
          {
            // does nothing
          }
    
    /*======================================================================*/
    /*! 
     *   Check, if the next Frame is available (that means, that the
     *   next call to updateImageData will not block)
     *
     *   \return true: there is a new Frame available
     */
    /*======================================================================*/

    bool isNextFrameAvailable()
          {
            return true;
          }
    
    
    void releaseImageData()
          {}
  
  private:
    Rgb32BitColor onePixelImage;
    
  };
} 
#endif //BASICDUMMYFRAMEGRABBER_HH
