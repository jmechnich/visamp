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

#ifndef SUBIMAGE_HH
#define SUBIMAGE_HH

#include <iostream>
#include <vector>

#include "RgbColor.hh"
#include "SubImageIterator.hh"
#include "MyError.hh"

namespace imrec
{

/*=====================================================================*/
/*!
 *  \class SubImage (SubImage.hh)
 *  \brief The SubImage template class provides methods to access a 
 *         rectangular area within a given image. 
 *
 *  The SubImage template class provides methods to access a rectangular 
 *  area within a given image. It has all methods of the Image class, so
 *  it behaves like a "real" image. 
 */
/*=====================================================================*/
template <class ImageType>
class SubImage
{
public:
  /*---------------------------------------------------------------------
   *  Iterator of base image
   *---------------------------------------------------------------------*/
  typedef typename ImageType::iterator base_iterator;
  
  /*---------------------------------------------------------------------
   *  Iterators and references
   *---------------------------------------------------------------------*/
  typedef typename ImageType::value_type value_type;
  typedef imrec::SubImageIterator<imrec::SubImage<ImageType> > iterator;
  typedef imrec::SubImageIterator<imrec::SubImage<ImageType> > 
  const_iterator;
  typedef typename ImageType::reference reference;
  typedef typename ImageType::const_reference const_reference;
  
  /*-----------------------------------------------------------------------
   *  Error class
   *-----------------------------------------------------------------------*/
  class WriteError:public MyError {public: WriteError(std::string s):MyError(s){}};

  /*===================================================================*/
  /*! 
   *   Constructor
   *
   *   \param image  pointer to base image
   *   \param top    y coordinate of top left pixel of subimage
   *   \param left   x coordinate of top left pixel of subimage
   *   \param nrows  height of subimage
   *   \param ncols  width of subimage
   */
  /*===================================================================*/
  SubImage(ImageType* image, 
           unsigned int top, unsigned int left, 
           unsigned int nrows, unsigned int ncols);
  
  
  /*===================================================================*/
  /*! 
   *   set new base image, position and dimensions
   *
   *   \param image  pointer to base image
   *   \param top    y coordinate of top left pixel of subimage
   *   \param left   x coordinate of top left pixel of subimage
   *   \param nrows  height of subimage
   *   \param ncols  width of subimage
   */
  /*===================================================================*/
  void set(ImageType* image, 
           unsigned int top, unsigned int left, 
           unsigned int nrows, unsigned int ncols);
  
  
  /*===================================================================*/
  /*! 
   *   move subimage
   *
   *   \param dy  vertical movement value
   *   \param dx  horizontal movement value
   */
  /*===================================================================*/ 
  void move(int dy, int dx);
  
  
  /*===================================================================*/
  /*! 
   *   resize subimage
   *
   *   \param nrows  new height of subimage
   *   \param ncols  new width of subimage
   */
  /*===================================================================*/
  void resize(unsigned int nrows, unsigned int ncols);
  
  
  /*===================================================================*/
  /*! 
   *   move and resize subimage
   *
   *   \param top    new y coordinate of top left pixel of subimage
   *   \param left   new x coordinate of top left pixel of subimage
   *   \param nrows  new height of subimage
   *   \param ncols  new width of subimage
   */
  /*===================================================================*/
  void resize(unsigned int top, unsigned int left, 
              unsigned int nrows, unsigned int ncols);
  

  /*======================================================================*/
  /*! 
   *   write the image to the given stream in binary PPM format (image
   *   Header "P6")
   *
   *   The image is written to an intermediate format 
   *   RgbColor<unsigned char>.
   *   The global function
   *   convertColor(  const T& srccol,  RgbColor<unsigned char>& trgcol)
   *   has to exist.
   *
   *   \param os  output stream. Must be already opened for writing
   *
   *   \exception WriteError an error occured during
   *                         writing. WriteError::msg() contains the plain
   *                         text error message.
   */
  /*======================================================================*/
  void write( std::ostream &os);

  
  /*===================================================================*/
  /*! 
   *   begin of subimage
   *
   *   \return iterator to the first subimage pixel
   */
  /*===================================================================*/
  const_iterator begin() const;
  iterator begin();
  
  
  /*===================================================================*/
  /*! 
   *   end of subimage
   *
   *   \return iterator to the first pixel of first row behind the 
   *           subimage
   */
  /*===================================================================*/
  const_iterator end() const;
  iterator end();
  
  
  /*===================================================================*/
  /*! 
   *   unchecked access to a pixel (one-dimensional position)
   *
   *   \param position  pixel position
   *
   *   \return reference to the pixel at position position
   */
  /*===================================================================*/
  const_reference operator[](unsigned int position) const;
  reference operator[](unsigned int position);
  
  
  /*===================================================================*/
  /*! 
   *   unchecked access to a pixel (two-dimensional position)
   *
   *   \param row    image row
   *   \param col    image col
   *
   *   \return  reference to the pixel at position row,col
   */
  /*===================================================================*/
  const_reference operator()(unsigned int row, unsigned int col) const;
  reference operator()(unsigned int row, unsigned int col);
  
  
  /*===================================================================*/
  /*! 
   *   vertical position of top left subimage pixel
   *
   *   \return vertical position of top left subimage pixel
   */
  /*===================================================================*/    
  unsigned int top() const;
  
  
  /*===================================================================*/
  /*! 
   *   horizontal position of top left subimage pixel
   *
   *   \return hortizontal position of top left subimage pixel
   */
  /*===================================================================*/
  unsigned int left() const;
  
  
  /*===================================================================*/
  /*! 
   *   height of the subimage (number of rows)
   *
   *   \return subimage height
   */
  /*===================================================================*/    
  unsigned int nrows() const;
  
  
  /*===================================================================*/
  /*! 
   *   width of the subimage (number of columns)
   *
   *   \return subimage width
   */
  /*===================================================================*/
  unsigned int ncols() const;
  
  
  /*===================================================================*/
  /*! 
   *   size of the subimage (number of pixels)
   *
   *   \return number of pixels
   */
  /*===================================================================*/    
  unsigned int size() const;
  
  
  /*===================================================================*/
  /*! 
   *   iterator of the BASE IMAGE to the first pixel of a subimage row 
   *
   *   \param row  row number
   *
   *   \return subimage row start
   */
  /*===================================================================*/    
  base_iterator rowStart(unsigned int row) const;
  
  
  /*===================================================================*/
  /*! 
   *   iterator of the BASE IMAGE to the pixel behind the last pixel of 
   *   a subimage row 
   *
   *   \param row  row number
   *
   *   \return subimage row end
   */
  /*===================================================================*/  
  base_iterator rowEnd(unsigned int row) const;
  
protected:
  //pointer to the base image
  ImageType* p_image;
  //position and dimensions
  unsigned int p_top, p_left, p_nrows, p_ncols;
  //begin and end iterators
  iterator p_begin;
  iterator p_end;
  //vectors of base image iterators pointing to begin and end of each row
  std::vector<base_iterator> p_rowStart, p_rowEnd;
  
};
  
#include "SubImage.icc"

}

#endif //SUBIMAGE_HH
