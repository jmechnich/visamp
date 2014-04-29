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

#ifndef RGBCOLOR_HH
#define RGBCOLOR_HH

#include <iostream>
#include <complex>

namespace imrec
{
  
/*======================================================================*/
/*!
 *  \class RgbColor RgbColor.hh
 *  \brief The RgbColor template class specifies a color with 
 *         red green blue components. 
 *
 *  it contains all methods for calculating with colors. For this the 
 *  basic type T must provide the corresponding arithmetic operators. 
 */
/*======================================================================*/
template <class T> 
class RgbColor
{

public:

  /*-----------------------------------------------------------------------
   *  Fixed colors by name
   *-----------------------------------------------------------------------*/
  static const RgbColor<T> White;
  static const RgbColor<T> Black;

  /*======================================================================*/
  /*! 
   *   Default Constructor creating a black (0,0,0) Color
   */
  /*======================================================================*/
  RgbColor ();

  /*======================================================================*/
  /*! 
   *    Constructor for creating an abitrary color
   *
   *   \param red     red component for color (0-255) resp. (0-1)
   *   \param green   green component for color (0-255) resp. (0-1)
   *   \param blue    blue component for color (0-255) resp. (0-1)
   */
  /*======================================================================*/
  RgbColor (T red, T green, T blue);

  /*======================================================================*/
  /*! 
   *   Constructor for creating a gray color (where red = green = blue)
   *
   *   \param gray   gray value (0-255) resp. (0-1)
   */
  /*======================================================================*/
  RgbColor (T gray);


  /*======================================================================*/
  /*! 
   *   Methods for accessing the color components   
   *
   *   \return colorvalue  value of assigned color component (0-255)
   *                       resp. (0-1)
   */
  /*======================================================================*/
  T red   () const;
  T green () const;
  T blue  () const;
  
  /*======================================================================*/
  /*! 
   *   Method for setting the color components   
   *
   *   \param red     red component for color (0-255) resp. (0-1)
   *   \param green   green component for color (0-255) resp. (0-1)
   *   \param blue    blue component for color (0-255) resp. (0-1)
   */
  /*======================================================================*/
  void set (T red, T green, T blue);

  /*======================================================================*/
  /*! 
   *   arithmetic operators (without range-checking)   
   */
  /*======================================================================*/
  RgbColor& operator += (const RgbColor& c);
  RgbColor& operator += (const T gray);
  RgbColor& operator -= (const RgbColor& c);
  RgbColor& operator -= (const T gray);
  RgbColor& operator *= (float factor);
  
  /*======================================================================*/
  /*! 
   *   boolean operators   
   */
  /*======================================================================*/
  bool operator == (const RgbColor& c) const;
  bool operator != (const RgbColor& c) const;


protected:

  T rc, gc, bc;
};

/*======================================================================*/
/*! 
 *   arithmetic operators   
 */
/*======================================================================*/
template <class T>
RgbColor<T> operator ! (const RgbColor<T>& c);

template <class T>
RgbColor<T> operator + (const RgbColor<T>& c1, const RgbColor<T>& c2);

template <class T>
RgbColor<T> operator - (const RgbColor<T>& c1, const RgbColor<T>& c2);

template <class T>
RgbColor<T> operator * (const RgbColor<T>& c1, float factor); 

template <class T>
RgbColor<T> operator * (float factor, const RgbColor<T>& c1);

/*======================================================================*/
/*! 
 *   << - overloading   
 */
/*======================================================================*/
template <class T>
std::ostream& operator << (std::ostream&, const RgbColor<T>&);

typedef unsigned char uchar;
typedef RgbColor<uchar>         bRGBColor;
typedef RgbColor<float>         fRGBColor;
  typedef RgbColor<std::complex<float> > cRGBColor;


template <class T>
std::ostream& operator << (std::ostream& os, const RgbColor<T>& c);

#include "RgbColor.icc"
}

#endif //RGBCOLOR_HH








