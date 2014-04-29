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

#ifndef RGB32BITCOLOR_HH
#define RGB32BITCOLOR_HH

#include "types.hh"

namespace imrec
{
  
/*======================================================================*/
/*! 
 *   Color class using a 32bit value to keep an RGB triple
 */
/*======================================================================*/

struct Rgb32BitColor
{
  uint32 data;
};
 
}


#endif //RGB32BITCOLOR_HH
