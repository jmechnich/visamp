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

#ifndef BLUESCREENWID_HH
#define BLUESCREENWID_HH


//STD
#include <fstream>

//QT
#include <QTimer>
#include <QColor>
#include <QString>
#include <QPainter>
#include <QPicture>

//LIBIMREC
#include "BasicFrameGrabber.hh"
#include "Image.hh"
#include "ConstantColor.hh"
#include "BlueScreen.hh"
#include "RgbColor.hh"
#include "ImageOverlayWid.hh"
#include "BsDistance.hh"
#include "convertColor.hh"
#include "ImageButton.hh"

namespace imrec
{
/*======================================================================*/
/*!
 *  \class BlueScreenWid BlueScreenWid.hh 
 *  \brief The BlueScreenWid class is a QT Widget that displays Images
 *  delivered by a BasicFrameGrabber
 *
 *  When the Widget is instantiated it has to be given a reference to a
 *  BasicFrameGrabber object. BlueScreenWid automatically polls new frames
 *  from the frame grabber and filters it using the BlueScreen effect. 
 *  The Widget offers different methods to change the BlueScreen behaviour
 *  capturing of images.
 */
/*======================================================================*/

  class BlueScreenWid : public imrec::ImageOverlayWid
  {

    Q_OBJECT
  public:


/*======================================================================*/
/*! 
 *   Constructor that gets a reference to a BasicFrameGrabber
 *
 *   \param grabber Reference to an BasicFrameGrabber
 * 
 */
/*======================================================================*/

    BlueScreenWid(imrec::BasicFrameGrabber* grabber, QWidget *parent=0);


/*======================================================================*/
/*! 
 *   Destructor
 */
/*======================================================================*/

      ~BlueScreenWid();


/*======================================================================*/
/*! 
 *   Returns the actual mask color
 *
 *   \return maskcolor actual mask color
 */
/*======================================================================*/

    QColor getColor();

/*======================================================================*/
/*! 
 *   Add Button to the Widget
 *
 *   \param Reference to an ImageButton 
 *
 */
/*======================================================================*/

    void addButton(imrec::ImageButton *imbutt);

/*======================================================================*/
/*! 
 *   Returns the number of Buttons which have been added
 *
 *   \return Number of Buttons in the Vector
 */
/*======================================================================*/

    int numberOfButtons();

    
public slots:

/*======================================================================*/
/*! 
 *   This slot is used to grab an Image and append it to a file
 *
 */
/*======================================================================*/
   void captureFrame();


/*======================================================================*/
/*! 
 *   If this slot is used, the actual Image will furtheron be used as
 *   reference background for the BlueScreen effect
 *
 */
/*======================================================================*/

    void setReference();


/*======================================================================*/
/*! 
 *   this slot opens a File Dialog where you can choose a certain PPM
 *   file that will be used as reference background
 *
 *   \exception IOException and Wrong File Format Exception
 *
 */
/*======================================================================*/

    void setFileReference(const QString& filereference);

  
/*======================================================================*/
/*! 
 *   This slot can be connected to a QTSlider Widget to set the tolerance
 *   value of the bluescreen effect
 *  void setTolerance(int tolerance_value);
 *   \param tolerance_value An int value that comes from the slider widget
 *   it should be between 0 an 255
 *
 */
/*======================================================================*/

    void setTolerance(int tolerance_value);

  
/*======================================================================*/
/*! 
 *   The RefreshRatio sets the time period between two image updates 
 *   
 *
 *   \param int time_period time in milliseconds between two Image
 *   update calls 
 *
 */
/*======================================================================*/
  
    void setRefreshRatio(unsigned int time_period);

  
/*======================================================================*/
/*! 
 *   processes the next frame and refreshes the Image
 *
 */
/*======================================================================*/

    virtual void refreshImage();

  
/*======================================================================*/
/*! 
 *   Switches the bluescreen effect on or off
 *
 *   \param blue_on bluescreen on or of
 *
 */
/*======================================================================*/

    void toggleBluescreen(bool blue_on);

/*======================================================================*/
/*! 
 *   Switches between viewing the widget with or without bluescreen effect
 *
 *   \param view_on Boolean value: true shows the bluescreen effect
 *
 */
/*======================================================================*/

    void toggleViewMode(bool view_on);
    


/*======================================================================*/
/*! 
 *   set the filename for the grabbed images
 *
 *   \param filename filename where images will be appended
 *
 */
/*======================================================================*/
  
    void setFilename(const QString& filename);


/*======================================================================*/
/*! 
 *   set the color from a QColor
 *
 *   \param color ColorValue for the background 
 *
 */ 
/*======================================================================*/

    void setColor(QColor color);


  private:
    imrec::BasicFrameGrabber *bfg;
    imrec::Image<imrec::RgbColor<uchar> > actualImage;
    imrec::Image<imrec::RgbColor<uchar> > referenceImage;
    imrec::Image<imrec::RgbColor<uchar> > capturedImage;
    imrec::Image<imrec::RgbColor<uchar> > cameraImage;
    imrec::BlueScreen<imrec::BsDistance> bluescr;
    imrec::ConstantColor<imrec::RgbColor<uchar> > *constantBackground;
    QColor background_color;
    
    QTimer *imageRefreshTimer;
  
  
    bool b_bluescreen;
    bool b_showTrueImage;
    
    int i_refreshRatio;

    QString p_filename;
    QString p_filereference;
    
    std::vector<imrec::ImageButton*> vecimbutt;
    
    int p_nrows;
    int p_ncols;
  };

}
#endif //BLUESCREENWID_HH








