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

#ifndef FILEFRAMEGRABBER_HH
#define FILEFRAMEGRABBER_HH

#include "RgbColor.hh"
#include "Image.hh"

namespace imrec
{
  /*======================================================================*/
  /*!
   *  \class FileFrameGrabber FileFrameGrabber.hh
   *  \brief The FileFrameGrabber class specifies a "frame grabber"
   *          who reads its frames from a file 
   *
   *  the file must contain a sequence of binary PPM images.
   */
  /*======================================================================*/
  class FileFrameGrabber : public Image<RgbColor<uchar> >
  {
  public:
    /*======================================================================*/
    /*! 
     *   Constructor 
     *
     */
    /*======================================================================*/
    FileFrameGrabber( const char* filename="test.bpm");

    /*======================================================================*/
    /*! 
     *   Constructor 
     *
     *   \param is  input stream containing a sequence of binary PPM images.
     *              must not be closed, before destruction of this class
     */
    /*======================================================================*/
    FileFrameGrabber( std::istream &is);
    
    void openFile( std::istream &is);
    
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
     *   read the next frame from disk
     */
    /*======================================================================*/
    void updateImageData();
    
    bool isNextFrameAvailable();
    
  private:
    std::istream* imageStream;
  };


#include "FileFrameGrabber.icc"
  
}


#endif //FILEFRAMEGRABBER_HH
