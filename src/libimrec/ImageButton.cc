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

#include "ImageButton.hh"

#ifdef DEBUG_LIBIMREC
#include <iostream>
#endif //DEBUG_LIBIMREC

imrec::ImageButton::ImageButton(unsigned int xpos,unsigned int ypos, 
                                unsigned int ncols, unsigned int nrows, 
                                QObject * parent, const char * name) 
        : imrec::RecognizeButton<imrec::Image<imrec::RgbColor<uchar> > >(xpos,ypos,ncols,nrows,name)
{
#ifdef DEBUG_LIBIMREC
  std::cout << "Image Button Name: " << this->name() << endl;
#endif //DEBUG_LIBIMREC
}

//-----------------------------------------------------------------------

imrec::ImageButton::~ImageButton()
{}

//-----------------------------------------------------------------------

void imrec::ImageButton::repaint(QPainter& p,
                                 imrec::Image<imrec::RgbColor<uchar> >  &to_repaint_im)
{
   p.setPen(QColor(255,0,0));
   p.drawRect(p_xpos,p_ypos,p_ncols,p_nrows);
}

//-----------------------------------------------------------------------

void imrec::ImageButton::setmaskcolor(QColor color)
{
  imrec::RecognizeButton<imrec::Image<imrec::RgbColor<uchar> > >::setmaskcolor(imrec::RgbColor<uchar>(color.red(),color.green(),color.blue()));
}

//-----------------------------------------------------------------------

void imrec::ImageButton::bpressed()
{
  emit pressed();
}

//-----------------------------------------------------------------------

void imrec::ImageButton::breleased()
{
  emit released();
}

//-----------------------------------------------------------------------

void imrec::ImageButton::bclicked()
{
  emit clicked();
}

//-----------------------------------------------------------------------

void imrec::ImageButton::setActivationThreshold(int level)
{
  bsetActivationThreshold(level);
#ifdef DEBUG_LIBIMREC
  std::cout << "Activation Threshold changed-" << p_button_name <<": " << i_activation_threshold << endl;
#endif //DEBUG_LIBIMREC

}

//-----------------------------------------------------------------------

void imrec::ImageButton::setDeactivationThreshold(int level)
{
  bsetDeactivationThreshold(level);

#ifdef DEBUG_LIBIMREC
  std::cout << "Activation Threshold changed-" << p_button_name <<": " << i_activation_threshold << endl;
#endif //DEBUG_LIBIMREC

}













