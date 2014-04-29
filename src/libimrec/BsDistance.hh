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

#ifndef BSDISTANCE_HH
#define BSDISTANCE_HH

namespace imrec
{
  
  class BsDistance
  {
    
  public:
    
    //! Distance "function". Calculates the square of the euklidian distance
    //! between two given pixel. 
    //!
    //! \param pixel1 reference to first pixel
    //! \param pixel2 reference to second pixel
    //! \return square of euklidian distance
    
    template <class ColorType_a, class ColorType_b>
    float operator()(ColorType_a& pixel1,
                     ColorType_b& pixel2);
          
  };
  
#include <BsDistance.icc>
}

#endif //BSDISTANCE_HH





