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

#ifndef CONVERTCOLOR_HH
#define CONVERTCOLOR_HH

#include "types.hh"
#include "Rgb16BitColor.hh"
#include "Rgb24BitColor.hh"
#include "Rgb32BitColor.hh"
#include "RgbColor.hh"
#include "ConversionTables.hh"

namespace imrec
{
  
/*======================================================================*/
/*! 
 *   General RGB color conversion
 *
 *   \param src  pointer to source color
 *   \param trg  pointer to target color
 *
 */
/*======================================================================*/
template< class srcColor, class trgColor>
inline
void convertColor( const srcColor& src, trgColor& trg)
{
  trg.set( src.red(), src.green(), src.blue());
}


/*-------------------------------------------------------------------------
 *  Conversions to Rgb16BitColor
 *-------------------------------------------------------------------------*/
template< class srcColorClass>
inline
void convertColor( const srcColorClass& src, Rgb16BitColor& trg)
{
  trg.data = ConvTab.red16( src.red()) 
      | ConvTab.green16( src.green()) 
      | ConvTab.blue16( src.blue());
}

inline
void convertColor( const Rgb32BitColor& src, Rgb16BitColor& trg)
{
  trg.data = src.data;
}

/*-------------------------------------------------------------------------
 *  Conversions to Rgb24BitColor
 *-------------------------------------------------------------------------*/
template< class srcColorClass>
inline
void convertColor( const srcColorClass& src, Rgb24BitColor& trg)
{
  trg.data = ConvTab.red32( src.red()) 
      | ConvTab.green32( src.green()) 
      | ConvTab.blue32( src.blue());
}

inline
void convertColor( const Rgb32BitColor& src, Rgb24BitColor& trg)
{
  trg.data = src.data;
}


/*-------------------------------------------------------------------------
 *  Conversions to Rgb32BitColor
 *-------------------------------------------------------------------------*/
template< class srcColorClass>
inline
void convertColor( const srcColorClass& src, Rgb32BitColor& trg)
{
  trg.data = ConvTab.red32( src.red()) 
      | ConvTab.green32( src.green()) 
      | ConvTab.blue32( src.blue());
}


inline
void convertColor( const Rgb32BitColor& src, Rgb32BitColor& trg)
{
  trg.data = src.data;
}

/*-------------------------------------------------------------------------
 *  Conversions to RgbColor<>
 *-------------------------------------------------------------------------*/
template< class componentType>
inline
void convertColor( const Rgb32BitColor& src, RgbColor<componentType>& trg)
{
  trg.set( uchar((src.data >> 16) & 0xff),
            uchar((src.data >> 8)  & 0xff),
            uchar((src.data)       & 0xff));
}



inline 
void convertColor ( const RgbColor<uchar>& src, RgbColor<uchar>& tgt)
{ 
  tgt = src;
}

inline 
void convertColor (const RgbColor<uchar>& src, float& tgt)
{
  tgt = ( ((float)src.red() * 0.30) + 
          ((float)src.green() * 0.59) + 
          ((float)src.blue() * 0.11) ); 
}

inline 
void convertColor (const RgbColor<uchar>& src, std::complex<float>& tgt )
{
  tgt = ( ((float)src.red() * 0.30) + 
          ((float)src.green() * 0.59) + 
          ((float)src.blue() * 0.11) ); 
}



inline 
void convertColor (const float& src, RgbColor<uchar>& tgt )
{ 
  tgt.set((uchar)( src ), (uchar)( src ), (uchar)( src ));
}


inline
void convertColor (const RgbColor<uchar>& src, RgbColor<float>& tgt)
{
  tgt.set (
      src.red(), 
      src.green(), 
      src.blue()
  );
}

inline 
void convertColor (const RgbColor<uchar>& src, RgbColor<std::complex<float> >& tgt)
{
  tgt.set(
       src.red(), 
       src.green(), 
       src.blue()
      );
}

inline 
void convertColor (const RgbColor<float>& src,  float& tgt)
{
  tgt = ( (src.red() * 0.30) + 
          (src.green() * 0.59) + 
          (src.blue() * 0.11) ); 
}


inline
void convertColor (const RgbColor<float>& src, RgbColor<uchar>& tgt)
{
  tgt.set( (uchar)( src.red()), 
           (uchar)( src.green()), 
           (uchar)( src.blue()) );
}

inline
void convertColor (const RgbColor<std::complex<float> >& src, RgbColor<uchar>& tgt)
{
  std::cerr << "tutnichts" << std::endl;
}

} //end of namespace

#endif //CONVERTCOLOR_HH
