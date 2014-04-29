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

#ifndef IMAGEBUTTON_HH
#define IMAGEBUTTON_HH

//QT
#include <qobject.h>
#include <qpainter.h>
#include <qcolor.h>

//LIBIMREC
#include "Image.hh"
#include "SubImage.hh"
#include "RgbColor.hh"
#include "RecognizeButton.hh"


namespace imrec
{
  
/*======================================================================*/
/*!
 *  \class ImageButton ImageButton.hh
 *  \brief The ImageButton class provides a Button 
 *
 *  The ImageButton is passed a region of a BlusScreenFiltered Image
 *  It checks the region if the number of pixels different to the maskcolor
 *  is above or below the threshold values.
 *  
 */
/*======================================================================*/

  
  class ImageButton : public QObject,
                      public imrec::RecognizeButton<imrec::Image<RgbColor<uchar> > >
  {

    
    Q_OBJECT
  public:

/*======================================================================*/
/*! 
 *   Constructor creates a Button with upper left corner at (x,y) and with 
 *   width w and height h.
 *
 *   \param im reference to an Image
 *
 *   \param xpos topleft position for the button
 *
 *   \param ypos y position of button
 *
 *   \param ncols width of the button
 *
 *   \param nrows height of the button
 */
/*======================================================================*/

    ImageButton(unsigned int xpos, unsigned int ypos,
                unsigned int ncols, unsigned int nrows,
                QObject * parent=0, const char * name=0);


/*======================================================================*/
/*! 
 *   Destructor
 */
/*======================================================================*/

    virtual ~ImageButton();


 /*======================================================================*/
/*! 
 *   Repaint method: You can use it to overwrite it in child classes from
 *   ImageButton. The method is invoked by the BlueScreen widget everytime 
 *   there is the need to repaint the Button.
 *
 *   \param p reference to a QPainter object
 *
 *   \param to_repaint_im reference to an Image on which will be painted
 *
 */
/*======================================================================*/

    virtual void repaint(QPainter& p,
                         imrec::Image<imrec::RgbColor<uchar> > &to_repaint_im);
    


 public slots:

 /*======================================================================*/
/*! 
 *   Set the mask color 
 *
 *   \param color QColor which will be the mask color
 *
 */
/*======================================================================*/

 void setmaskcolor(QColor color);


/*======================================================================*/
/*! 
 *   This slot sets threshold (number of pixels different from maskcolor)
 *   of activation
 *
 *   \param level Number of pixel that have to be different from maskcolor
 *
 */
/*======================================================================*/

    void setActivationThreshold(int level);


/*======================================================================*/
/*! 
 *   This slot sets threshold (number of pixels different from maskcolor)
 *   of deactivation
 *
 *   \param level Number of pixel that have to be different from maskcolor
 *
 */
/*======================================================================*/

    void setDeactivationThreshold(int level);
    
    
signals:

/*======================================================================*/
/*! 
 *   Signal pressed which will be emitted when the activation in
 *   the button region rises above the threshold  
 */
/*======================================================================*/

    void pressed();

/*======================================================================*/
/*! 
 *   Signal released will be emitted when the activation in the button 
 *   region falls below the deactivation threshold
 */
/*======================================================================*/

    void released();

/*======================================================================*/
/*! 
 *   Signal clicked will be emitted when after one sequence of emitting 
 *   pressed and released
 *
 */
/*======================================================================*/

    void clicked();
    

  protected:
    
    
 /*======================================================================*/
/*! 
 *   This signal is emitted when the button is pressed down. 
 *
 */
/*======================================================================*/
    
    virtual void bpressed();

    
 /*======================================================================*/
/*! 
 *   This signal is emitted when the button is released. 
 *
 */
/*======================================================================*/

    virtual void breleased();


 /*======================================================================*/
/*! 
 *   This signal is emitted when the button is activated, i.e. first pressed 
 *   down and then released when the cursor is inside the button.
 *
 */
/*======================================================================*/

    virtual void bclicked();
  };
}
#endif //IMAGEBUTTON_HH



