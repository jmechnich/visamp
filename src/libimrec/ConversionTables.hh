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

#ifndef CONVERSIONTABLES_HH
#define CONVERSIONTABLES_HH

#include "types.hh"

namespace imrec
{

/*======================================================================*/
/*!
 *  \class ConversionTables ConversionTables.hh
 *  \brief The ConversionTables class is used for converting
 *         Colors to Display-accuracy. 
 *
 *  One global instance of this class is generated for performing 
 *  these conversions.
 */
/*======================================================================*/


class ConversionTables
{
public:
/*======================================================================*/
/*! 
 *   Constructor, initializes the internal conversion tables
 *
 */
/*======================================================================*/

  ConversionTables();

/*======================================================================*/
/*! 
 *   Conversion of 8-Bit color value to display-specific 16-bit value
 *
 *   \param red   8-bit red component
 *
 *   \return 16-bit value
 */
/*======================================================================*/

  uint16 red16(   uchar red)   const {return red16Table[red];}
  uint16 green16( uchar green) const {return green16Table[green];}
  uint16 blue16(  uchar blue)  const {return blue16Table[blue];}

/*======================================================================*/
/*! 
 *   Conversion of 8-Bit color value to display-specific 32-bit value
 *
 *   \param red   8-bit red component
 *
 *   \return 32-bit value
 */
/*======================================================================*/

  uint32 red32(   uchar red)   const {return red32Table[red];}
  uint32 green32( uchar green) const {return green32Table[green];}
  uint32 blue32(  uchar blue)  const {return blue32Table[blue];}
  
private:
  uint16 red16Table[256];
  uint16 green16Table[256];
  uint16 blue16Table[256];

  uint32 red32Table[256];
  uint32 green32Table[256];
  uint32 blue32Table[256];
  
};

/*-------------------------------------------------------------------------
 *  Global conversion table
 *-------------------------------------------------------------------------*/
extern const ConversionTables ConvTab;
 
}

#endif //CONVERSIONTABLES_HH


