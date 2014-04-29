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

#ifndef CONSTANTCOLOR_HH
#define CONSTANTCOLOR_HH

#include <iterator>

namespace imrec 
{

/*======================================================================*/
/*!
 *  \class ConstantColor ConstantColor.hh
 *  \brief The ConstantColor class is an iterator which iterates through
 *         an virtualy infinite image of constant color
 */
/*======================================================================*/
template<typename ColorType>
class ConstantColor : public std::iterator<std::forward_iterator_tag,
                                           ColorType,
                                           int>
{
public:
  /*====================================================================*/
  /*! 
   *   constructor
   *
   *   \param color initial color value
   */
  /*====================================================================*/
  ConstantColor(ColorType color);
  

  /*====================================================================*/
  /*! 
   *   prefix operator++ 
   *
   *   \return reference to constant color
   */
  /*====================================================================*/
  ConstantColor<ColorType>& operator++();
  

  /*====================================================================*/
  /*! 
   *   postfix operator++ 
   *
   *   \return reference to constant color
   */
  /*====================================================================*/
  ConstantColor<ColorType>& operator++(int);
  

  /*======================================================================*/
  /*!  
   *   operator*  
   *
   *   \return reference to ColorType
   */
  /*======================================================================*/
  ColorType& operator*();
  const ColorType& operator*() const;
  
protected:
  ColorType p_color;
};

#include "ConstantColor.icc"

}

#endif //CONSTANTCOLOR_HH





