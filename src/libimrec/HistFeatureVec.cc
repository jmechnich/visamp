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

#include "HistFeatureVec.hh"

imrec::HistFeatureVec::HistFeatureVec(
    int bins, 
    imrec::RgbColor<unsigned char> mask)
        :p_mask(mask), p_bin(bins)
{
  p_size = 256/p_bin;
  resize();
}

//-----------------------------------------------------------------------

imrec::HistFeatureVec::~HistFeatureVec()
{}

//-----------------------------------------------------------------------

void imrec::HistFeatureVec::resize()
{
  data.resize( int( pow(double (p_bin), 3.)));
  ystart.resize( int( pow( double(p_bin), 2.)));
  zstart.resize( p_bin);
  
  iterator it_data = data.begin();  
  for( size_type y = 0; y < ystart.size(); y++)
  {
    ystart[y] = it_data;
    it_data += p_bin;
  }

  std::vector<iterator>::iterator it_ystart = ystart.begin();  
  for( size_type z = 0; z < zstart.size(); z++)
  {
    zstart[z] = it_ystart;
    it_ystart += p_bin;
  }
}











