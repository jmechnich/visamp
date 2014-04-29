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

#ifndef Image_HH
#define Image_HH


#ifndef __THROW_BAD_ALLOC
#include <new>
#define __THROW_BAD_ALLOC throw bad_alloc()
#endif

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>

#include "RgbColor.hh"
#include "MyError.hh"
#include "convertColor.hh"


namespace imrec
{
  
/*======================================================================*/
/*!
 *  \class Image  Image.hh
 *  \brief The Image class is a template image class for
 *  storing and manipulating RGB and gray-value-images.  
 *  
 *  It contains methods for loading and saving PPM images and for
 *  direct access to the pixels. The basic pixel-type can be arbitrary, e.g.
 *  float, int, unsigned char, RgbColor<float>, 
 *  RgbColor<double>... 
 *  For writing/reading the existence of 
 *  global functions 
 *  convertColor( const T& srccol, RgbColor<unsigned char>& trgcol)
 *  resp. 
 *  convertColor( const RgbColor<unsigned char>& srccol, T& trgcol)
 *  is assumed. For some standard combinations RgbColor.hh
 *  provides these.
 */
/*======================================================================*/
template <class T>
class Image 
{
  
public:
  
  /*-----------------------------------------------------------------------
   *  Error classes
   *-----------------------------------------------------------------------*/
  class ReadError: public MyError {public: ReadError(std::string s): MyError(s){}};
  class WriteError:public MyError {public: WriteError(std::string s):MyError(s){}};
  class OutOfBoundsError:public MyError {public: OutOfBoundsError():MyError(""){}};
  
  /*-----------------------------------------------------------------------
   *  Iterators and references
   *-----------------------------------------------------------------------*/
  typedef T                                   value_type;
  typedef typename std::vector<T>::iterator        iterator;
  typedef typename std::vector<T>::const_iterator  const_iterator;
  typedef typename std::vector<T>::reference       reference;
  typedef typename std::vector<T>::const_reference const_reference;
  typedef typename std::vector<T>::size_type       size_type;
  
  /*======================================================================*/
  /*! 
   *   Default Constructor. Creates an empty image
   */
  /*======================================================================*/
  Image();
  
  /*======================================================================*/
  /*! 
   *   Destructor
   */
  /*======================================================================*/ 
  ~Image();

  /*======================================================================*/
  /*! 
   *   resize image to nrows * ncols. If it has already this size, nothing
   *   is done 
   *
   *   \param nrows   new number of rows
   *   \param ncols   new number of columns
   */
  /*======================================================================*/ 
  void resize(unsigned int nrows, unsigned int ncols);

  /*======================================================================*/
  /*! 
   *   read an image from the given stream in binary PPM format (image
   *   Header "P6")
   *
   *   the image is read to an intermediate format RgbColor<unsigned char>
   *   the global function
   *   convertColor( const RgbColor<unsigned char>& srccol, T& trgcol)
   *   must exist.
   *
   *   \param is   input stream. Must be already opened for reading,
   *               with the read postition pointing to the beginning of
   *               the image header 
   *
   *   \exception ReadError an error occured during
   *                        reading. ReadError::msg() contains the plain
   *                        text error message.
   */
  /*======================================================================*/
  void read( std::istream &is);

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

  /*======================================================================*/
  /*! 
   *   unchecked access to a pixel (two-dimensional position)
   *
   *   \param row    image row
   *   \param col    image col
   *
   *   \return  reference to the pixel at position row,col
   */
  /*======================================================================*/
  reference operator()(unsigned int row, unsigned int col);
  const_reference operator()(unsigned int row, unsigned int col) const;
  
  /*======================================================================*/
  /*! 
   *   width of the image (number of columns)
   *
   *   \return image width
   */
  /*======================================================================*/
  unsigned int ncols() const;

  /*======================================================================*/
  /*! 
   *   height of the image (number of rows)
   *
   *   \return image height
   */
  /*======================================================================*/
  unsigned int nrows() const;

  /*======================================================================*/
  /*! 
   *   size of the image (number of pixels)
   *
   *   \return number of pixels
   */
  /*======================================================================*/
  unsigned int size() const;

  /*======================================================================*/
  /*! 
   *   begin of image
   *
   *   \return pointer to first pixel of image
   */
  /*======================================================================*/
  iterator       begin();
  const_iterator begin() const;

  /*======================================================================*/
  /*! 
   *   end of image
   *
   *   \return pointer to the first pixel BEHIND the last one 
   *           of the image
   */
  /*======================================================================*/
  iterator       end();
  const_iterator end() const;
  
  /*======================================================================*/
  /*! 
   *   operator[] 
   *
   *   \param n    pixel number
   *   \return reference to pixel number n of image
   */
  /*======================================================================*/
  reference       operator[]( unsigned int n); 
  const_reference operator[]( unsigned int n) const;
  
private:
  
  unsigned int      ncolumns;
  std::vector<T> data;
  std::vector<iterator>  rowStart;
  
};

#include "Image.icc"
}

#endif //IMAGE_HH









