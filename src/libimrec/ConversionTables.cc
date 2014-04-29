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

#include "ConversionTables.hh"


namespace imrec
{
  //Global conversion table
  const ConversionTables ConvTab;
}

//-----------------------------------------------------------------------

imrec::ConversionTables::ConversionTables()
{
  for( int i = 0; i < 256; i++)
  {
    red16Table[i] = (i & 0xf8) << 8;
    green16Table[i] = (i & 0xfc) << 3;
    blue16Table[i] = (i & 0xf8) >> 3;

    red32Table[i]   = i << 16;
    green32Table[i] = i << 8;
    blue32Table[i]  = i;
  }
}



