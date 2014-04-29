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

#include "MyRecognizer.hh"

void MyRecognizer::repaint( QPainter& p, rgbImage& image) const
{
    p.setPen(QColor(0,0,255));
    p.drawRect( p_left, p_top, p_ncols, p_nrows);
}

void MyRecognizer::recognized()
{
    int index    = recognizedIndex();
    QString name = getName( index).c_str();
    
    emit recognizedSig();
        
    if( lastIndex != index){
        emit recIndex( index);
        emit recName ( name);
        emit recChanged();
        lastIndex = index;
    }
}
