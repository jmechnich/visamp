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

#ifndef MYSLIDER_HH
#define MYSLIDER_HH

#include <qpainter.h>

#include <RgbColor.hh>
#include <Image.hh>
#include <Slider.hh>

typedef unsigned int uint;

class MySlider : public imrec::Slider<imrec::Image<imrec::RgbColor<uchar> > >
{
public:
    MySlider( uint left, uint top, uint width, uint height) : imrec::Slider<imrec::Image<imrec::RgbColor<uchar> > >
    ( left, top, width, height)
        {}

    void repaint( QPainter& p, imrec::Image<imrec::RgbColor<uchar> > & img);
};

#endif
