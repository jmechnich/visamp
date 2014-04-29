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

#ifndef IMAGEOVERLAYWID_HH
#define IMAGEOVERLAYWID_HH

#include <qpicture.h>
#include <qpainter.h>

#include "FastImageDisplayWid.hh"

namespace imrec
{
  class ImageOverlayWid : public FastImageDisplayWid
  {
  public:
    ImageOverlayWid(QWidget * parent=0,
                    Qt::WindowFlags f = 0 )
            : FastImageDisplayWid(parent, f)
          {}
        
    void setOverlayGraphics( const QPicture& pic)
          {
            overlay = pic;
          }

  protected:
    virtual void paintEvent( QPaintEvent *pe )
          {
            FastImageDisplayWid::paintEvent( pe);
            QPainter p( this);
            p.drawPicture( 0, 0, overlay);
          }

  private:
    QPicture overlay;
  };
}

#endif //IMAGEOVERLAYWID_HH

