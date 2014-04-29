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

#include "MyImageButtons.hh"

void PlayButton::repaint( QPainter& p, imrec::Image<imrec::RgbColor<uchar> >& im)
{
    imrec::ImageButton::repaint( p, im);
    QBrush br = p.brush();
    p.setPen(   QColor( 255, 0, 0) );
    p.setBrush( QColor( 255, 0, 0) );
    QPolygon qpa;
    qpa.setPoints( 3,( p_xpos + ( p_ncols/3) ), ( p_ypos + ( p_nrows/3) ), ( p_xpos + ( p_ncols/3) ), 
                   ( p_ypos + ( 2*( p_nrows/3)) ), ( p_xpos + ( 2*( p_ncols/3)) ), ( p_ypos + ( p_nrows/2)) );
    p.drawPolygon( qpa);
    p.setBrush( br);
}


void PauseButton::repaint( QPainter& p, imrec::Image<imrec::RgbColor<uchar> >& im)
{
    imrec::ImageButton::repaint( p, im);
    QBrush br = p.brush();
    p.setPen(   QColor( 255, 0, 0) );
    p.setBrush( QColor( 255, 0, 0) );
    p.drawRect( ( p_xpos + ( p_ncols/3) ), ( p_ypos + ( p_nrows/3) ), ( p_ncols/9), ( p_nrows/3) );
    p.drawRect( ( p_xpos + ( 5*( p_ncols/9)) ), ( p_ypos + ( p_nrows/3) ), ( p_ncols/9), ( p_nrows/3) );
    p.setBrush( br);
}

void StopButton::repaint( QPainter& p, imrec::Image<imrec::RgbColor<uchar> >& im)
{
    imrec::ImageButton::repaint( p, im);
    QBrush br = p.brush();
    p.setPen(   QColor( 255, 0, 0) );
    p.setBrush( QColor( 255, 0, 0) );
    p.drawRect( ( p_xpos + ( p_ncols/3) ), ( p_ypos + ( p_nrows/3) ), ( p_ncols/3), ( p_nrows/3) );
    p.setBrush( br);
}

void FwdButton::repaint( QPainter& p, imrec::Image<imrec::RgbColor<uchar> >& im)
{
    imrec::ImageButton::repaint( p, im);
    QBrush br = p.brush();
    p.setPen(   QColor( 255, 0, 0) );
    p.setBrush( QColor( 255, 0, 0) );
    QPolygon qpa;
    qpa.setPoints( 3, ( p_xpos + ( p_ncols/3) ), ( p_ypos + ( p_nrows/3) ), ( p_xpos + ( p_ncols/3) ),
                   ( p_ypos + ( 2*( p_nrows/3)) ), ( p_xpos + ( 5*( p_ncols/9)) ), ( p_ypos + ( p_nrows/2)) );
    p.drawPolygon( qpa);
    p.drawRect( ( p_xpos + ( 5*( p_ncols/9)) ), ( p_ypos + ( p_nrows/3) ), ( p_ncols/9), ( p_nrows/3) );
    p.setBrush( br);
}

void BckButton::repaint( QPainter& p, imrec::Image<imrec::RgbColor<uchar> >& im)
{
  imrec::ImageButton::repaint( p, im);
  QBrush br = p.brush();
  p.setPen(   QColor( 255, 0, 0) );
  p.setBrush( QColor( 255, 0, 0) );
  QPolygon qpa;
  qpa.setPoints( 3, (p_xpos + 2*( p_ncols/3)), (p_ypos + p_nrows/3),
                 (p_xpos + 2*(p_ncols/3)), (p_ypos + 2*( p_nrows/3)),
                 (p_xpos + 4*( p_ncols/9)), (p_ypos + p_nrows/2));
  p.drawPolygon( qpa);
  p.drawRect( ( p_xpos + ( p_ncols/3) ), ( p_ypos + ( p_nrows/3) ), ( p_ncols/9), ( p_nrows/3) );
  p.setBrush( br);
}
