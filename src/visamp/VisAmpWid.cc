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

#include <QApplication>
#include <QString>
#include <QFileDialog>
#include <QPushButton>
#include <QMenuBar>
#include <QPainter>
#include <QPicture>
#include <QInputDialog>
#include <QMessageBox>

#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <signal.h>

#include "VisAmpWid.hh"

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:  Constructor
 *  ==> see headerfile
 *=======================================================================*/
VisAmpWid::VisAmpWid()
        : maskColor(255, 255, 255), col(255, 255, 255), bsCol(col),
          mirrorImage( false)
{
  setWindowTitle( "VisAmp");
  
  std::string devicePath = "/dev/video0";
  std::string playerPath = "/usr/bin/mpg123";
  std::string filePath   = "";
  
  size_t argc = qApp->argc();
  for( size_t i=1; i < argc; ++i)
  {
    std::string data = qApp->argv()[i];
    if( data == "-h" || data == "--help")
    {
      std::cout << "usage: visamp [options] [filename]\n"
                << "   Options:\n"
                << "    -d | --device  " << "path to device\n"
                << "    -p | --player  " << "path to commandline mp3 player\n";
      std::exit(0);
    }
    if( data == "-d" || data == "--device")
    {
      if( i+1 > argc)
      {
        std::cerr << "Missing argument" << std::endl;
        qApp->quit();
      }
      devicePath = qApp->argv()[++i];
      continue;
    }
    else if( data == "-p" || data == "--player")
    {
      if( i+1 > argc)
      {
        std::cerr << "Missing argument" << std::endl;
        qApp->quit();
      }
      playerPath = qApp->argv()[++i];
      continue;
    }
    else
    {
      if( i+1 != argc)
      {
        std::cerr << "Stray argument '" << qApp->argv()[i] << "'" << std::endl;
        qApp->quit();
      }
      filePath = qApp->argv()[i];
      continue;
    }
  }
  frameGrabber = new imrec::BasicFrameGrabber( devicePath.c_str());
  
/*-------------------------------------------------------------------------
 *  Initializing Bluescreen and Images
 *-------------------------------------------------------------------------*/
  fg_rows = frameGrabber->nrows();
  fg_cols = frameGrabber->ncols();
  
  BS.setTolerance(30);
  fidWid = new imrec::ImageOverlayWid(this);
  fidWid->setImageSize( fg_rows, fg_cols);
  
  /*-----------------------------------------------------------------------
   *  Allocate our images
   *-----------------------------------------------------------------------*/
  workingImg.resize( fg_rows, fg_cols);
  bsOut.resize     ( fg_rows, fg_cols);
  bgr.resize       ( fg_rows, fg_cols);
  
/*-------------------------------------------------------------------------
 *  Initializing Player Object
 *-------------------------------------------------------------------------*/
  if( !filePath.size())
  {
    QString fileName = QFileDialog::getOpenFileName(
        this, "Open File...", QString::null,
        "Music/Playlist Files (*.mp3 *.m3u *.txt)");
    
    filePath = fileName.toStdString();
  }
  player = new imrec::Player( playerPath.c_str(), filePath.c_str());
    
/*-------------------------------------------------------------------------
 *  Initializing Slider Object
 *-------------------------------------------------------------------------*/
  slider = new MySlider(fg_rows-40,0,160,40);

/*-------------------------------------------------------------------------
 *  Initializing ImageButton Objects
 *-------------------------------------------------------------------------*/
  prevImgButton = new BckButton  ( 0,  40, 40, 40);
  playImgButton = new PlayButton ( 0,   0, 40, 40);
  pausImgButton = new PauseButton( 0, 160, 40, 40);
  stopImgButton = new StopButton ( 0, 120, 40, 40);
  nextImgButton = new FwdButton  ( 0,  80, 40, 40);
  
  connect( prevImgButton, SIGNAL( clicked()), this, SLOT( previous()));
  connect( playImgButton, SIGNAL( clicked()), this, SLOT( play()));
  connect( pausImgButton, SIGNAL( clicked()), this, SLOT( pause()));
  connect( stopImgButton, SIGNAL( clicked()), this, SLOT( stop()));
  connect( nextImgButton, SIGNAL( clicked()), this, SLOT( next()));
  
/*-------------------------------------------------------------------------
 *  Initializing Recognizer Object
 *-------------------------------------------------------------------------*/
  rec_top   = 0;
  rec_left  = 200;
  rec_nrows = 100;
  rec_ncols = 120;
  
  recognizer = new MyRecognizer(
      bsOut, rec_top, rec_left, rec_nrows, rec_ncols, *bsCol, 0.5, 5, 4, true);
  
  connect( recognizer, SIGNAL( recIndex( int)), this, SLOT( play( int)));
  
/*-------------------------------------------------------------------------
 *  Create Control Panels
 *-------------------------------------------------------------------------*/
  /*-----------------------------------------------------------------------
   *  Create Slider Options Widget
   *-----------------------------------------------------------------------*/
  QWidget* sliderOptions = new QWidget;
  sliderOptions->setWindowTitle( "Slider");
  sliderStatus = new QCheckBox( "Slider On/Off");
  QPushButton* slSetMinButton = new QPushButton( "Set Minimum");
  QPushButton* slSetMaxButton = new QPushButton( "Set Maximum");
  minvalLabel  = new QLabel ( "Minimum:");
  minValSlider = new QSlider( Qt::Horizontal);
  maxvalLabel  = new QLabel ( "Maximum:");
  maxValSlider = new QSlider( Qt::Horizontal);
  
  QVBoxLayout* sliderOptionsLayout = new QVBoxLayout;
  sliderOptionsLayout->addWidget( sliderStatus);
  sliderOptionsLayout->addWidget( slSetMinButton);
  sliderOptionsLayout->addWidget( slSetMaxButton);
  sliderOptionsLayout->addWidget( minvalLabel);
  sliderOptionsLayout->addWidget( minValSlider);
  sliderOptionsLayout->addWidget( maxvalLabel);
  sliderOptionsLayout->addWidget( maxValSlider);
  sliderOptions->setLayout( sliderOptionsLayout);
  
  sliderStatus->setChecked( false);
  connect( slSetMinButton, SIGNAL( clicked()), this, SLOT( calibrateMin())) ;
  connect( slSetMaxButton, SIGNAL( clicked()), this, SLOT( calibrateMax())) ;
    
  minValSlider->setMinimum(   0);
  minValSlider->setValue(    50);
  minValSlider->setMaximum( 100);
  minValSlider->setPageStep( 1);
  setSliderMinVal(50);
  maxValSlider->setMinimum(   0);
  maxValSlider->setValue(   100);
  maxValSlider->setMaximum( 100);
  maxValSlider->setPageStep( 1);
  setSliderMaxVal(100);

  connect( minValSlider, SIGNAL( sliderMoved(int)), this, SLOT( setSliderMinVal(int)));
  connect( maxValSlider, SIGNAL( sliderMoved(int)), this, SLOT( setSliderMaxVal(int)));
    

  /*-------------------------------------------------------------------------
   *  Create ImageButton Options Widget
   *-------------------------------------------------------------------------*/
  QWidget* imgButtOptions = new QWidget;
  imgButtOptions->setWindowTitle("ImageButtons");
    
  imgButtonStatus = new QCheckBox( "ImageButtons On/Off");
  activLabel      = new QLabel ( "Activation Threshold:");
  activSlider     = new QSlider( Qt::Horizontal);
  deactLabel      = new QLabel ( "Deactivation Threshold:");
  deactSlider     = new QSlider( Qt::Horizontal);
  statusBar       = new QStatusBar;
    
  QVBoxLayout* imgButtOptionsLayout = new QVBoxLayout;
  imgButtOptionsLayout->addWidget( imgButtonStatus);
  imgButtOptionsLayout->addWidget( activLabel);
  imgButtOptionsLayout->addWidget( activSlider);
  imgButtOptionsLayout->addWidget( deactLabel);
  imgButtOptionsLayout->addWidget( deactSlider);
  imgButtOptionsLayout->addWidget( statusBar);
  imgButtOptions->setLayout( imgButtOptionsLayout);
    
  imgButtonStatus->setChecked(false);
  setActivThresh(50);
  setDeactThresh(50);
    
  activSlider->setMinimum( 0);
  activSlider->setMaximum( 100);
  activSlider->setPageStep( 1);
  activSlider->setValue( 50);
  connect( activSlider, SIGNAL( sliderMoved(int)), this, SLOT( setActivThresh(int)));
  deactSlider->setMinimum( 0);
  deactSlider->setMaximum( 100);
  deactSlider->setPageStep( 1);
  deactSlider->setValue( 50);
  connect( deactSlider, SIGNAL( sliderMoved(int)), this, SLOT( setDeactThresh(int)));

  statusBar->setSizeGripEnabled ( false);

  /*-----------------------------------------------------------------------
   *  Create Recognizer Options Widget
   *-----------------------------------------------------------------------*/
  QWidget* recognizerOptions = new QWidget;
  recognizerOptions->setWindowTitle( "Recognizer");
    
  recognizerStatus = new QCheckBox( "Recognizer On/Off");
  QPushButton* recLearnButton     = new QPushButton( "Learn");
  QPushButton* recLearnFileButton = new QPushButton( "Learn From File");
  recClTolLabel  = new QLabel ( "Classifier Tolerance:");
  recClTolSlider = new QSlider( Qt::Horizontal);
  recStatusBar   = new QStatusBar;
    
  QVBoxLayout* recognizerOptionsLayout = new QVBoxLayout;
  recognizerOptionsLayout->addWidget( recognizerStatus);
  recognizerOptionsLayout->addWidget( recLearnButton);
  recognizerOptionsLayout->addWidget( recLearnFileButton);
  recognizerOptionsLayout->addWidget( recClTolLabel);
  recognizerOptionsLayout->addWidget( recClTolSlider);
  recognizerOptionsLayout->addWidget( recStatusBar);
  recognizerOptions->setLayout( recognizerOptionsLayout);
    
  recognizerStatus->setChecked( false);
  connect( recLearnButton    , SIGNAL( clicked()), this, SLOT( learn()    ));
  connect( recLearnFileButton, SIGNAL( clicked()), this, SLOT( learnFile()));
  recClTolSlider->setMinimum(   0);
  recClTolSlider->setValue(    30);
  recClTolSlider->setMaximum( 200);
  recClTolSlider->setPageStep( 5);
  setRecognizerClTol( 30);
  connect( recClTolSlider, SIGNAL( sliderMoved(int)), this, SLOT( setRecognizerClTol(int)));
        
  recStatusBar->setSizeGripEnabled ( false);

  recImg    = new QWidget;
  recImgWid = new imrec::ImageOverlayWid;

  QVBoxLayout* recImgLayout = new QVBoxLayout;
  recImgLayout->addWidget( recImgWid);
  recImg->setLayout( recImgLayout);
    
  recImgWid->setImageSize( rec_nrows, rec_ncols);

  /*-----------------------------------------------------------------------
   *  Create and update Playlist Widget
   *-----------------------------------------------------------------------*/
  playlist = new QListWidget;
  playlist->setWindowTitle( "Playlist");
  playlist->setFixedSize( 400, 200);

  connect( playlist, SIGNAL( itemActivated( QListWidgetItem*)),
           this, SLOT( play( QListWidgetItem*)));
  connect( player, SIGNAL( playlistChanged()), this, SLOT( updatePlaylist()));
  connect( player, SIGNAL( currentIndex( int)), this, SLOT( songChanged( int)));
    
  updatePlaylist();
    
  playlist->setCurrentItem( 0);
    
/*-------------------------------------------------------------------------
 *  Initializing Main Widget
 *-------------------------------------------------------------------------*/
  /*-----------------------------------------------------------------------
   *  Create Menubar
   *-----------------------------------------------------------------------*/
  QMenuBar *menuBar = new QMenuBar(0);
  QAction* action;
  QMenu *file = menuBar->addMenu( tr("&File"));
    
  action = new QAction( tr("&Open File"), this);
  action->setShortcut( Qt::CTRL + Qt::Key_O);
  connect( action, SIGNAL( triggered()), this, SLOT( open()));
  file->addAction( action);
    
  action = new QAction( tr("&Add File"), this);
  action->setShortcut( Qt::CTRL + Qt::Key_A);
  connect( action, SIGNAL( triggered()), this, SLOT( add()));
  file->addAction( action);

  action = new QAction( tr("&Save Playlist"), this);
  action->setShortcut( Qt::CTRL + Qt::Key_S);
  connect( action, SIGNAL( triggered()), this, SLOT( save()));
  file->addAction( action);
  file->addSeparator();
    
  action = new QAction( tr("E&xit"), this);
  action->setShortcut( Qt::CTRL + Qt::Key_Q);
  connect( action, SIGNAL( triggered()), qApp, SLOT( quit()));
  file->addAction( action);

  QMenu* windows = menuBar->addMenu( tr("&Windows"));
  action = new QAction( tr("&Slider"), this);
  connect( action, SIGNAL( triggered()), sliderOptions, SLOT( show()));
  windows->addAction( action);

  action = new QAction( tr("&ImageButtons"), this);
  connect( action, SIGNAL( triggered()), imgButtOptions, SLOT( show()));
  windows->addAction( action);
    
  action = new QAction( tr("&Recognizer"), this);
  connect( action, SIGNAL( triggered()), recognizerOptions, SLOT( show()));
  windows->addAction( action);
  windows->addSeparator();
    
  action = new QAction( tr("&Mirror Image"), this);
  action->setShortcut( Qt::CTRL + Qt::Key_M);
  connect( action, SIGNAL( triggered()), this, SLOT( toggleMirror()));
  windows->addAction( action);
    
  action = new QAction( tr("&Playlist"), this);
  action->setShortcut( Qt::CTRL + Qt::Key_P);
  connect( action, SIGNAL( triggered()), playlist, SLOT( show()));
  windows->addAction( action);
    
  menuBar->addSeparator();
    
  QMenu *help = menuBar->addMenu(tr("&Help"));
  action = new QAction( tr("&About"), this);
  action->setShortcut( Qt::CTRL + Qt::Key_H);
  connect( action, SIGNAL( triggered()), this, SLOT( about()));
  windows->addAction( action);

  /*-----------------------------------------------------------------------
   *  Create Main Layout
   *-----------------------------------------------------------------------*/
  layout = new QHBoxLayout( this);

  /*-----------------------------------------------------------------------
   *  Add Menubar to Main Layout
   *-----------------------------------------------------------------------*/
  layout->setMenuBar( menuBar);
    
  /*-----------------------------------------------------------------------
   *  Add ImageOverlayWidget to Main Layout
   *-----------------------------------------------------------------------*/
  layout->addWidget( fidWid);
  
  /*-----------------------------------------------------------------------
   *  Create buttons for Player Control
   *-----------------------------------------------------------------------*/
  QPushButton* playButton  = new QPushButton( "Play"    , this);
  QPushButton* stopButton  = new QPushButton( "Stop"    , this);
  QPushButton* pauseButton = new QPushButton( "Pause"   , this);
  QPushButton* prevButton  = new QPushButton( "Previous", this);
  QPushButton* nextButton  = new QPushButton( "Next"    , this);

  connect( playButton , SIGNAL( clicked()), this, SLOT( play()    ));
  connect( stopButton , SIGNAL( clicked()), this, SLOT( stop()    ));
  connect( pauseButton, SIGNAL( clicked()), this, SLOT( pause()   ));
  connect( prevButton , SIGNAL( clicked()), this, SLOT( previous()));
  connect( nextButton , SIGNAL( clicked()), this, SLOT( next()    ));
    
  /*-----------------------------------------------------------------------
   *  Create sliders for Player Control
   *-----------------------------------------------------------------------*/
  volLabel   = new QLabel ( "Volume:", this);
  volSlider  = new QSlider( Qt::Horizontal, this);
  volSlider->setMinimum( 0);
  volSlider->setMaximum( 100);
  volSlider->setPageStep( 5);
  volSlider->setValue( 50);
    
  connect( volSlider, SIGNAL( valueChanged(int)), this, SLOT( setVolume(int)));

  /*-----------------------------------------------------------------------
   *  Add Player Control to Layout
   *-----------------------------------------------------------------------*/

  QVBoxLayout* controlLay = new QVBoxLayout;

  QHBoxLayout* playLay = new QHBoxLayout;
  playLay->addWidget( playButton );
  playLay->addWidget( pauseButton);
  playLay->addWidget( stopButton );
  controlLay->addLayout( playLay);
    
  QHBoxLayout* transpLay = new QHBoxLayout;
  transpLay->addWidget( prevButton);
  transpLay->addWidget( nextButton);
  controlLay->addLayout( transpLay);
    
  controlLay->addStretch( 1);
    
  controlLay->addWidget( volLabel );
  controlLay->addWidget( volSlider);

  controlLay->addStretch( 1);

  showOverlay = new QCheckBox( "Overlay Graphics On/Off", this);
  showOverlay->setChecked( false);
  controlLay->addWidget( showOverlay);

  controlLay->addStretch( 1);
  layout->addLayout( controlLay);
    
  /*-----------------------------------------------------------------------
   *  Add Bluescreen Control to Layout
   *-----------------------------------------------------------------------*/
  BsCheckBox = new QCheckBox( "Bluescreen On/Off", this);
  BsCheckBox->setChecked( false);
  controlLay->addWidget( BsCheckBox);
    
  QPushButton* slSetBackButton = new QPushButton( "Set Background", this);
  bsTolLabel       = new QLabel   ( "BS-Tolerance:"                   , this);
  BlueScreenSlider = new QSlider  ( Qt::Horizontal, this);
  BlueScreenSlider->setMinimum( 0);
  BlueScreenSlider->setMaximum( 100);
  BlueScreenSlider->setPageStep( 5);
  BlueScreenSlider->setValue( 50);
    
  connect( slSetBackButton, SIGNAL( clicked()), this, SLOT( setBackground()));
  connect( BlueScreenSlider, SIGNAL( sliderMoved(int)), this, SLOT( setBsTolerance(int))) ;
  connect( BlueScreenSlider, SIGNAL( sliderMoved(int)), this, SLOT( setBSLabel(int)))     ;

  controlLay->addWidget( slSetBackButton);
  controlLay->addWidget( bsTolLabel);
  controlLay->addWidget( BlueScreenSlider);
    
  this->setFixedSize( this->sizeHint());
  playlist->show();

/*-------------------------------------------------------------------------
 *  Initializing processTimer
 *-------------------------------------------------------------------------*/
  processTimer = new QTimer();
  connect( processTimer, SIGNAL( timeout()), this, SLOT( processNextFrame()));
  processTimer->start( 50);

  connect( qApp, SIGNAL( aboutToQuit()), this, SLOT(quit()));
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:  processNextFrame()
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::processNextFrame()
{
  if( frameGrabber->isNextFrameAvailable())
  {
        
    /*---------------------------------------------------------------------
     *  Process images
     *---------------------------------------------------------------------*/
    frameGrabber->updateImageData();

    /*---------------------------------------------------------------------
     *  Copy Frame into our image
     *----------------------------------------------------------------------*/
    imrec::BasicFrameGrabber::const_iterator p = frameGrabber->imageBegin();
    if( !mirrorImage)
    {
      imrec::Image<imrec::RgbColor<uchar> >::iterator q = workingImg.begin();
      for( ; p!= frameGrabber->imageEnd(); ++p)
      {
        imrec::convertColor( *p, *q);
        ++q;
      }
    }
    else
    {
      for( unsigned int row=0; row < fg_rows; ++row)
      {
        for( unsigned int col=fg_cols; col > 0 ; --col)
        {
          imrec::convertColor( *p, workingImg(row,col-1));
          ++p;
        }
      }
    }
    
    frameGrabber->releaseImageData();
    
    BS.process( workingImg.begin(), workingImg.end(),
                bgr.begin(), bsCol, bsOut.begin() );
    
    /*-----------------------------------------------------------------------
     *  pass image to FastImageDisplayWid
     *-----------------------------------------------------------------------*/
    if( BsCheckBox->isChecked()){
      fidWid->setImage( bsOut.begin(), bsOut.end());
    } else {
      fidWid->setImage( workingImg.begin(), workingImg.end());
    }
      
/*-------------------------------------------------------------------------
 *  Process Slider, ImageButtons and Recognizer ( if activated)
 *-------------------------------------------------------------------------*/

    /*-----------------------------------------------------------------------
     *  update Slider
     *-----------------------------------------------------------------------*/
    if( sliderStatus->isChecked()){
      slider->setImage(&bsOut, col);
      slider->process();
         
      volSlider->setValue(slider->value());
    } 

    /*-----------------------------------------------------------------------
     *  update ImageButtons
     *-----------------------------------------------------------------------*/
    if( imgButtonStatus->isChecked()){
      prevImgButton->setmaskcolor( maskColor);
      prevImgButton->setImage(&bsOut);
      prevImgButton->process();

      playImgButton->setmaskcolor( maskColor);
      playImgButton->setImage(&bsOut);
      playImgButton->process();
        
      pausImgButton->setmaskcolor( maskColor);
      pausImgButton->setImage(&bsOut);
      pausImgButton->process();
        
      stopImgButton->setmaskcolor( maskColor);
      stopImgButton->setImage(&bsOut);
      stopImgButton->process();
        
      nextImgButton->setmaskcolor( maskColor);
      nextImgButton->setImage(&bsOut);
      nextImgButton->process();
    }

    /*-----------------------------------------------------------------------
     *  update Recognizer
     *-----------------------------------------------------------------------*/
    if( recognizerStatus->isChecked()){
      recognizer->process();
      refreshRecImg( recognizer->getImage( recognizer->recognizedIndex()));
            
      ostringstream os;
      if(recognizer->recognizedIndex()!=-1)
      {
        os << recognizer->getName(recognizer->recognizedIndex()) 
           << ", " 
           << recognizer->getLastDistance();
      }
      else
      {
        os << "Nothing, " 
           << recognizer->getLastDistance();
      }
      recStatusBar->showMessage( os.str().c_str());
    } else {
      recStatusBar->showMessage( "");
      recImg->hide();
    }
                
    /*-----------------------------------------------------------------------
     *  paint OverlayGraphics ( if showOverlay is checked)
     *-----------------------------------------------------------------------*/
    QPicture pic;
    pic.setData( NULL, 0);
    fidWid->setOverlayGraphics( pic);

    if( showOverlay->isChecked()){
      QPainter paint;
      paint.begin( &pic);
      if( sliderStatus->isChecked()){
        slider->repaint( paint, bsOut);
      }
      if( imgButtonStatus->isChecked()){
        prevImgButton->repaint( paint, bsOut);
        playImgButton->repaint( paint, bsOut);
        pausImgButton->repaint( paint, bsOut);
        stopImgButton->repaint( paint, bsOut);
        nextImgButton->repaint( paint, bsOut);
      }
      if( recognizerStatus->isChecked()){
        recognizer->repaint( paint, bsOut);
      }
            
      paint.end();
      fidWid->setOverlayGraphics( pic);
    }
  }
}


/*-------------------------------------------------------------------------
 *  slots responsible for menubar actions
 *-------------------------------------------------------------------------*/
/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::open()
{
  QString fileName = QFileDialog::getOpenFileName( 
      this, "Open File...", QString::null, "Music/Playlist Files (*.mp3 *.m3u *.txt)");
  if ( !fileName.isNull() ) {
    stop();
    delete player;
    ostringstream fname;
    fname << fileName.toStdString();
    player = new imrec::Player((qApp->argv())[1], fname.str().c_str());
    connect( player, SIGNAL( playlistChanged()), this, SLOT( updatePlaylist()));
    updatePlaylist();
    playlist->setCurrentItem( 0);
    play();
  }
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::add()
{
  QString fileName = QFileDialog::getOpenFileName(
      this, "Add File...", QString::null, "Music/Playlist Files (*.mp3 *.m3u *.txt)");
  if ( !fileName.isNull() ) {                 // got a file name
    player->addFile( fileName);
  }
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::save()
{
  QString fileName = QFileDialog::getSaveFileName( this, "Save File...", "Newlist.m3u", "Playlists (*.m3u)");
  if ( !fileName.isNull() ) {                 // got a file name
    player->saveList( fileName);
  }
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::quit()
{
  stop();
}


/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::about()
{
  QMessageBox::information( this, "About", "VisAmp V1.0");
}

/*-------------------------------------------------------------------------
 *  slots responsible for communication with Player class
 *-------------------------------------------------------------------------*/
/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::play()
{
  play( playlist->currentRow());
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::play( QListWidgetItem* item)
{
  play( playlist->row(item));
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::play( int index)
{
  player->play( index);
  statusBar->showMessage( "Play", 2000);
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::pause()
{
  player->pause();
  statusBar->showMessage( "Pause", 2000);
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::stop()
{
  player->stop();
  statusBar->showMessage( "Stop", 2000);
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::previous()
{
  player->previous();
  playlist->setCurrentRow(player->getCurrent());
  statusBar->showMessage( "Previous", 2000);
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::next()
{
  player->next();
  playlist->setCurrentRow(player->getCurrent());
  statusBar->showMessage( "Next", 2000);
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::setVolume( int volume)
{
  ostringstream os;
  os << "Volume: " 
     << volume;
  volLabel->setText( os.str().c_str());
  player->setVolume( volume);
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::updatePlaylist()
{
  playlist->clear();
    
  imrec::Player::iterator it;
  for( it = player->begin(); it != player->end(); it++){
    playlist->addItem( *it);
  }    
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::songChanged( int index)
{
  playlist->setCurrentRow( index);
}

/*-------------------------------------------------------------------------
 *  slots responsible for communication with Slider class
 *-------------------------------------------------------------------------*/
/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::setSliderMinVal(int val)
{
  slider->setMinVal(val);
  ostringstream os;
  os << "Minimum: " 
     <<  val;
  minvalLabel->setText( os.str().c_str()); 
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::setSliderMaxVal(int val)
{
  slider->setMaxVal(val);
  ostringstream os;
  os << "Maximum: " 
     <<  val;
  maxvalLabel->setText( os.str().c_str());
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::calibrateMin()
{
  slider->calibrateMin();
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::calibrateMax()
{
  slider->calibrateMax();
}

/*-------------------------------------------------------------------------
 *  slots responsible for communication with ImageButton class
 *-------------------------------------------------------------------------*/
/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::setActivThresh( int val)
{
  prevImgButton->setActivationThreshold( val);
  playImgButton->setActivationThreshold( val);
  pausImgButton->setActivationThreshold( val);
  stopImgButton->setActivationThreshold( val);
  nextImgButton->setActivationThreshold( val);
  ostringstream os;
  os << "Activation Threshold: " 
     <<  val;
  activLabel->setText( os.str().c_str()); 
}
    
/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::setDeactThresh( int val)
{
  prevImgButton->setDeactivationThreshold( val);
  playImgButton->setDeactivationThreshold( val);
  pausImgButton->setDeactivationThreshold( val);
  stopImgButton->setDeactivationThreshold( val);
  nextImgButton->setDeactivationThreshold( val);
  ostringstream os;
  os << "Deactivation Threshold: " 
     <<  val;
  deactLabel->setText( os.str().c_str()); 
}

/*-------------------------------------------------------------------------
 *  slots responsible for communication with Recognizer class
 *-------------------------------------------------------------------------*/
/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::setRecognizerClTol(int tol)
{
  float clTol = tol/100.;
    
  recognizer->setTolerance( clTol);
  ostringstream os;
  os << "Classifier Tolerance: "
     << clTol;
  recClTolLabel->setText( os.str().c_str());
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::learn()
{
  processTimer->stop();

  bool ok = false;
  QString name  = QInputDialog::getText( this, "Recognizer", "Name for Sample",
                                         QLineEdit::Normal, "samplename", &ok);
    
  if( ok)
  {
    string s_name = name.toStdString();
    int classID = QInputDialog::getInteger( this, "Recognizer", "Index for Sample", 0, 0, 2147483647, 1, &ok);
        
    if( ok){

      if ( QMessageBox::information( this, "Recognizer", "Do you want to save learned Image to disk", 
                                     "Yes", "No", 0, 1) ==0)
      {
        ostringstream os;
        os << classID << "_" << s_name << ".ppm";
        QString filename = QInputDialog::getText( this, "Recognizer", "Enter filename", QLineEdit::Normal, os.str().c_str());
        imrec::SubImage<imrec::Image<imrec::RgbColor<uchar> > >si
            ( &bsOut, rec_top, rec_left, rec_nrows, rec_ncols);
        ofstream file( filename.toStdString().c_str());
        si.write( file);
      }
      recognizer->learn( bsOut, classID, s_name);
    }
  }
  processTimer->start( 50);
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::learnFile()
{
  QString s = QFileDialog::getOpenFileName( this, "Learn File...", QString::null, "Sample Image (*.ppm)");

  if (!s.isNull()){
    QString nameDialog = s.right( s.length() - s.lastIndexOf('/') - 1);
    int pos=nameDialog.indexOf( '_');
  
    int classID=nameDialog.left( pos).toInt();
    nameDialog=nameDialog.mid( pos + 1, nameDialog.length() - 5 - pos);
    
    nameDialog=QInputDialog::getText( this, "Recognizer", "Name for Sample",
                                      QLineEdit::Normal, nameDialog); 
    string s_name=nameDialog.toStdString();
  
    classID=QInputDialog::getInteger( this, "Recognizer", "Index for Sample", classID, 0);
  
    ifstream file( s.toStdString().c_str());
    imrec::Image<imrec::RgbColor<uchar> > image;
    image.read( file);
  
    recognizer->learn( image, classID, s_name);
  }
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::refreshRecImg( const rgbImage& img)
{
  if ((img.nrows()!=0) && (img.ncols()!=0))
  {
    recImgWid->setImageSize(img.nrows(), img.ncols());
    recImgWid->setImage(img.begin(), img.end());
    recImg->show();
        
  }
  else
  {
    recImg->hide();
  }
}

/*-------------------------------------------------------------------------
 *  slots responsible for Bluescreen control
 *-------------------------------------------------------------------------*/
/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::setBSLabel(int tol)
{
  ostringstream os;
  os << "BS-Tolerance: " 
     <<               tol                ;
  bsTolLabel->setText( os.str().c_str());
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::setBsTolerance(int tol)
{
  BS.setTolerance(tol);
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::setBackground()
{
  while( !frameGrabber->isNextFrameAvailable());
  frameGrabber->updateImageData();
  bgr.resize( frameGrabber->nrows(), frameGrabber->ncols());
  imrec::BasicFrameGrabber::const_iterator p = frameGrabber->imageBegin();
  if( !mirrorImage)
  {
    imrec::Image<imrec::RgbColor<uchar> >::iterator q = bgr.begin();
    for( ; p!= frameGrabber->imageEnd(); p++) {
      imrec::convertColor(*p, *q);
      q++;
    }
  }
  else
  {
    for( unsigned int row=0; row < fg_rows; ++row)
    {
      for( unsigned int col=fg_cols; col > 0 ; --col)
      {
        imrec::convertColor( *p, bgr(row,col-1));
        ++p;
      }
    }
  }
    
  frameGrabber->releaseImageData();
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::toggleMirror()
{
  mirrorImage = !mirrorImage;
}

/*=========================================================================
 *  DESCRIPTION OF FUNCTION:
 *  ==> see headerfile
 *=======================================================================*/
void VisAmpWid::closeEvent( QCloseEvent* event)
{
  QWidget::closeEvent( event);
  qApp->quit();
}
