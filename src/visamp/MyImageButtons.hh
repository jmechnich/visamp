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

#ifndef MYIMAGEBUTTONS_HH
#define MYIMAGEBUTTONS_HH

#include <QPainter>
#include <QColor>
#include <QBrush>

#include <ImageButton.hh>


class PlayButton : public imrec::ImageButton
{
public:
  
    PlayButton( unsigned int x, unsigned int y, unsigned int ncols, unsigned int nrows,
                QObject * parent=0, const char * name=0) : imrec::ImageButton( x, y, ncols, nrows, parent, name)
        {}
    
    void repaint( QPainter& p, imrec::Image<imrec::RgbColor<uchar> >& im);
};


class PauseButton : public imrec::ImageButton
{
public:
  
  PauseButton( unsigned int x, unsigned int y, unsigned int ncols, unsigned int nrows,
               QObject * parent=0, const char * name=0) : imrec::ImageButton( x, y, ncols, nrows, parent, name)
        {}

    void repaint( QPainter& p, imrec::Image<imrec::RgbColor<uchar> >& im);
};


class StopButton : public imrec::ImageButton
{
public:
  
  StopButton( unsigned int x, unsigned int y, unsigned int ncols, unsigned int nrows,
              QObject * parent=0, const char * name=0) : imrec::ImageButton( x, y, ncols, nrows, parent, name)
        {}

    void repaint( QPainter& p, imrec::Image<imrec::RgbColor<uchar> >& im);
};


class FwdButton : public imrec::ImageButton
{
public:
  
  FwdButton(unsigned int x, unsigned int y, unsigned int ncols, unsigned int nrows,
            QObject * parent=0, const char * name=0) : imrec::ImageButton( x, y, ncols, nrows, parent, name)
        {}

  void repaint( QPainter& p, imrec::Image<imrec::RgbColor<uchar> >& im);
};


class BckButton : public imrec::ImageButton
{
public:
  
  BckButton( unsigned int x, unsigned int y, unsigned int ncols, unsigned int nrows,
             QObject * parent=0, const char * name=0) : imrec::ImageButton(x,y,ncols,nrows,parent,name)
        {}

  void repaint( QPainter& p, imrec::Image<imrec::RgbColor<uchar> >& im);
};

#endif
