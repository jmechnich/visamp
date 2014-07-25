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

#ifndef VISAMPWID_HH
#define VISAMPWID_HH

/*-------------------------------------------------------------------------
 *  Qt includes
 *-------------------------------------------------------------------------*/
#include <QWidget>
#include <QColor>
#include <QLayout>
#include <QStatusBar>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QTimer>
#include <QListWidget>

/*-------------------------------------------------------------------------
 *  Libimrec includes
 *-------------------------------------------------------------------------*/
#include <BasicFrameGrabber.hh>
#include <RgbColor.hh>
#include <ConstantColor.hh>
#include <ImageOverlayWid.hh>
#include <BlueScreen.hh>
#include <BsDistance.hh>
#include <Player.hh>
#include <Image.hh>

/*-------------------------------------------------------------------------
 *  Own includes
 *-------------------------------------------------------------------------*/
#include "MySlider.hh"
#include "MyImageButtons.hh"
#include "MyRecognizer.hh"

using namespace std;

typedef imrec::Image<imrec::RgbColor<uchar> > rgbImage;

class VisAmpWid : public QWidget
{
    Q_OBJECT
public:
/*======================================================================*/
/*! 
 *   Constructor
 */
/*======================================================================*/
    VisAmpWid();
    
private slots:

/*======================================================================*/
/*! 
 *   Read next frame from framegrabber, process it
 *    and pass it to OverlayDisplayWid
 */
/*======================================================================*/
    void processNextFrame();
    
public slots:

/*======================================================================*/
/*! 
 *   slots responsible for menubar actions
 */
/*======================================================================*/
    void open();
    void add();
    void save();
    void quit();
    void about();

/*======================================================================*/
/*! 
 *   slots responsible for communication with Player class
 */
/*======================================================================*/
    void play();
    void play( QListWidgetItem* item);
    void play( int index);
    void pause();
    void stop();
    void previous();
    void next();
    void setVolume( int volume);
    void updatePlaylist();
    void songChanged( int index);
    
/*======================================================================*/
/*! 
 *   slots responsible for communication with Slider class
 */
/*======================================================================*/
    
/*======================================================================*/
/*! 
 *   sets the minimum value of the Slider
 *   \param val  the minimum value of the slider
 */
/*======================================================================*/
    void setSliderMinVal(int val);
    
/*======================================================================*/
/*! 
 *  sets the maximum value of the Slider   
 *  \param val  the maximum value of the slider
 */
/*======================================================================*/
    void setSliderMaxVal(int val);
    
/*======================================================================*/
/*! 
 *  calibrates the position of the minimum slider state
 */
/*======================================================================*/
    void calibrateMin();
    
/*======================================================================*/
/*! 
 *   calibrates the position of the maximum slider state
 */
/*======================================================================*/
    void calibrateMax();

/*======================================================================*/
/*! 
 *   slots responsible for communication with ImageButton class
 */
/*======================================================================*/

/*======================================================================*/
/*! 
 *   sets the activation threshold of all ImageButtons
 *   /param val  the activation threshold of all ImageButtons
 */
/*======================================================================*/
    void setActivThresh( int val);
    
/*======================================================================*/
/*! 
 *   sets the deactivation threshold of all ImageButtons
 *   \param val  the deactivation threshold of all ImageButtons
 */
/*======================================================================*/
    void setDeactThresh( int val);
    
/*======================================================================*/
/*! 
 *   slots responsible for communication with Recognizer class
 */
/*======================================================================*/

/*======================================================================*/
/*! 
 *   sets the Classifier tolerance
 *   \param tol  the Classifier tolerance
 */
/*======================================================================*/
    void setRecognizerClTol(int tol);

/*======================================================================*/
/*! 
 *   open learn dialog
 */
/*======================================================================*/
    void learn();
    
/*======================================================================*/
/*! 
 *   open learn from file dialog
 */
/*======================================================================*/
    void learnFile();
    
/*======================================================================*/
/*! 
 *   shows and hides the recognized image widget
 */
/*======================================================================*/
    void refreshRecImg( const rgbImage& img);
    
/*======================================================================*/
/*! 
 *   slots responsible for Bluescreen control
 */
/*======================================================================*/

/*======================================================================*/
/*! 
 *   responsible for setting the right BlueScreen tolerance label
 */
/*======================================================================*/
    void setBSLabel(int tol);
    
/*======================================================================*/
/*! 
 *   sets the BlueScreen tolerance
 *   \param tol  the BlueScreen tolerance
 */
/*======================================================================*/
    void setBsTolerance(int tol);
    
/*======================================================================*/
/*! 
 *   sets the background needed for BlueScreen filtering
 */
/*======================================================================*/
    void setBackground();  

  void toggleMirror();
  
private:
  void closeEvent( QCloseEvent* event);
  
/*-------------------------------------------------------------------------
 *  Qt objects
 *-------------------------------------------------------------------------*/
    QColor       maskColor       ;
    QHBoxLayout* layout          ;
    QStatusBar*  statusBar       ;
    
    QLabel*      volLabel        ;
    QSlider*     volSlider       ;
    QCheckBox*   showOverlay     ;

    QCheckBox*   BsCheckBox      ;
    QLabel*      bsTolLabel      ;
    QSlider*     BlueScreenSlider;

    QCheckBox*   sliderStatus    ;
    QLabel*      minvalLabel     ;
    QSlider*     minValSlider    ;
    QLabel*      maxvalLabel     ;
    QSlider*     maxValSlider    ;

    QCheckBox*   imgButtonStatus ;
    QLabel*      activLabel      ;
    QSlider*     activSlider     ;
    QLabel*      deactLabel      ;
    QSlider*     deactSlider     ;

    QCheckBox*   recognizerStatus;
    QLabel*      recClTolLabel   ;
    QSlider*     recClTolSlider  ;
    QWidget*     recImg          ;
    QStatusBar*  recStatusBar    ;
        
    QListWidget* playlist        ;

    QTimer*      processTimer    ;

/*-------------------------------------------------------------------------
 *  Libimrec objects
 *-------------------------------------------------------------------------*/
    imrec::BasicFrameGrabber*                       frameGrabber ;   
    rgbImage                                        workingImg   ;     
    rgbImage                                        bsOut        ;          
    rgbImage                                        bgr          ;
    imrec::RgbColor<uchar>                          col          ;
    imrec::ConstantColor<imrec::RgbColor<uchar> >   bsCol        ;  
    imrec::ImageOverlayWid*                         fidWid       ;
    imrec::BlueScreen<imrec::BsDistance>            BS           ;
    imrec::Player*                                  player       ;
    imrec::ImageOverlayWid*                         recImgWid    ;

/*-------------------------------------------------------------------------
 *  Own objects
 *-------------------------------------------------------------------------*/
    MySlider*                                       slider       ;
    BckButton*                                      prevImgButton;
    PlayButton*                                     playImgButton;
    PauseButton*                                    pausImgButton;
    StopButton*                                     stopImgButton;
    FwdButton*                                      nextImgButton;
    MyRecognizer*                                   recognizer   ;

/*-------------------------------------------------------------------------
 *  Member variables
 *-------------------------------------------------------------------------*/
    int rec_top, rec_left, rec_nrows, rec_ncols;

  int fg_rows, fg_cols;
  
  bool mirrorImage;

};

#endif
