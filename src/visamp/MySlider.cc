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

#include "MySlider.hh"

void MySlider::repaint( QPainter& p, imrec::Image<imrec::RgbColor<uchar> > & image)
{
    p.setPen(QColor(0,255,0));
    p.drawRect(int(p_left), int( p_top), int(p_width), int(p_height));
    p.drawRect(int(p_currentX)+p_left, int(p_currentY)+p_top, 10, 10);
    p.drawRect(int(p_minX)+p_left,int( p_minY)+p_top,5,5);
    p.drawRect(int(p_maxX)+p_left,int(p_maxY)+p_top,5,5);
}
