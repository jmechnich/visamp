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

#ifndef BLUESCREEN_HH
#define BLUESCREEN_HH

#include "ConstantColor.hh"
#include "convertColor.hh"

namespace imrec
{

/*======================================================================*/
/*!
 *  \class BlueScreen BlueScreen.hh
 *  \brief The BlueScreen class provides methods to perform "blue box
 *         filtering" on certain input images. 
 *
 */
/*======================================================================*/  
template <class funcDistance>
class BlueScreen
{
public:
  /*======================================================================*/
  /*! 
   *   Constructor
   */
  /*======================================================================*/
  BlueScreen();
  

  /*======================================================================*/
  /*! 
   *   the setTolerance method sets the blue screen tolerance to a new
   *   value.
   *
   *   \param value new tolerance value 
   */
  /*======================================================================*/
  void setTolerance( float value );


  /*======================================================================*/
  /*! 
   *   the distance function calculates the distance between two colors.
   *
   *   \param pixel1 first pixel
   *   \param pixel2 second pixel
   *
   *   \return distance
   */
  /*======================================================================*/
  template <class PixelType1, class PixelType2>
  float distance(PixelType1 pixel1, PixelType2 pixel2);
  

  /*======================================================================*/
  /*! 
   *   the template process method replaces the actual background 
   *   image/color in the input image by a new background image/color.
   *
   *   \param srcBegin first pixel of input image
   *   \param srcEnd   pixel behind last pixel of input image
   *   \param oldBack  first pixel of actual background image or 
   *                   background color
   *   \param newBack  first pixel of new background image or 
   *                   background color 
   *   \param dstBegin first pixel of destination image  
   * 
   */
  /*======================================================================*/
  template <class SrcIterator, class DstIterator, 
            class OldBackType, class NewBackType>
  void process(SrcIterator srcBegin,
               SrcIterator srcEnd,
               OldBackType oldBack,
               NewBackType newBack,
               DstIterator dstBegin);

private:
  float p_tolerance;
  funcDistance p_dist;
};

#include "BlueScreen.icc"

}

#endif //BLUESCREEN_HH






