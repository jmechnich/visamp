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

#include "FastImageDisplayWid.hh"

#include <QX11Info>

#include <cstdlib>

#include "Rgb16BitColor.hh"
#include "Rgb24BitColor.hh"
#include "Rgb32BitColor.hh"

imrec::FastImageDisplayWid::FastImageDisplayWid( QWidget * parent, Qt::WindowFlags f)
        :QWidget( parent, f),
         xImage( 0)
{
  setAttribute( Qt::WA_NoBackground);
  setAttribute( Qt::WA_PaintOnScreen);
  /*-----------------------------------------------------------------------
   *  create the GC
   *-----------------------------------------------------------------------*/
  imageGC = XCreateGC(QX11Info::display(), winId(), 0, NULL);
}

//-----------------------------------------------------------------------

void imrec::FastImageDisplayWid::setImageSize( uint nrows, uint ncols)
{
  /*-----------------------------------------------------------------------
   *  Check if xImage needs to be resized
   *-----------------------------------------------------------------------*/
  if( xImage == 0 
      || nrows != (uint)xImage->height 
      || ncols != (uint)xImage->width)
  {
    if( xImage != 0)
    {
      XDestroyImage( xImage);
    }

    /*---------------------------------------------------------------------
     *  find out, how man bytes per pixel are required
     *---------------------------------------------------------------------*/
    int bytesPerPixel;
    int padding;
    
    switch( QX11Info::appDepth())
    {
    case 16:
      bytesPerPixel = 2;
      padding = 16;
      break;
    case 24:
    case 32:
      bytesPerPixel = 4;
      padding = 32;
      break;
    default:
    {
      std::ostringstream errmsg;
      errmsg << "Requested bit depth (" << QX11Info::appDepth()
             << ") not supported. "
          "Please Set your Screen to 16, 24 or 32 bits, or use command "
          "line argument '-visual TrueColor' on SGI's";
      throw UnsupportedBitDepth( errmsg.str());
    }
    }
    
    /*---------------------------------------------------------------------
     *  allocate data for image
     *---------------------------------------------------------------------*/
    char* data = (char*) malloc( nrows * ncols * bytesPerPixel);
    
    /*---------------------------------------------------------------------
     *  create an XImage (ownership of allocated data is passed to xImage)
     *---------------------------------------------------------------------*/
    xImage = XCreateImage(QX11Info::display(), 
                          (Visual*)QX11Info::appVisual(),
                          QX11Info::appDepth(), 
                          ZPixmap, 0, (char*)data, 
                          ncols, nrows, padding, 0);
  }

  /*-----------------------------------------------------------------------
   *  the QtWidget should not be resized to bigger sizes
   *-----------------------------------------------------------------------*/
  setMaximumSize( ncols, nrows);
  setMinimumSize( ncols, nrows);
  

}

//-----------------------------------------------------------------------

QSize imrec::FastImageDisplayWid::sizeHint() const
{
  return QSize( xImage->width, xImage->height);
}

//-----------------------------------------------------------------------

void imrec::FastImageDisplayWid::paintEvent( QPaintEvent * )
{
  /*-----------------------------------------------------------------------
   *  Just redraw the whole widget (don't take regions into accout)
   *-----------------------------------------------------------------------*/
  if( xImage != 0)
  {
    XPutImage(QX11Info::display(), winId(), imageGC, xImage,
              0, 0, 0, 0, xImage->width, xImage->height);
  }
  
}


