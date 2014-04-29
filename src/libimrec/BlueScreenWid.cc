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

#include "BlueScreenWid.hh"
#ifdef DEBUG_LIBIMREC
#include <iostream>
#endif //DEBUG_LIBIMREC



imrec::BlueScreenWid::BlueScreenWid(imrec::BasicFrameGrabber* grabber, QWidget *parent)
        : imrec::ImageOverlayWid( parent)
{
  bfg=grabber;
  
  p_nrows = bfg->nrows();
  p_ncols = bfg->ncols();
  
  // fix size to grabber image size
  setMinimumSize(p_nrows,p_ncols);
  setMaximumSize(p_nrows,p_ncols);


  //define a standard color for the background, close to black
  constantBackground = new imrec::ConstantColor<imrec::RgbColor<uchar> >(imrec::RgbColor<uchar>(1,1,1));
  background_color = QColor(1,1,1);  
  
  imageRefreshTimer = new QTimer(this);
  connect (imageRefreshTimer, SIGNAL(timeout()),this,SLOT(refreshImage()));

  //clear vector
  vecimbutt.clear();

  //resize images
  setImageSize(p_nrows,p_ncols);
  actualImage.resize(p_nrows,p_ncols);
  referenceImage.resize(p_nrows,p_ncols);
  cameraImage.resize(p_nrows,p_ncols);
  capturedImage.resize(p_nrows,p_ncols);

  bfg->updateImageData();
  setReference();
  
  
  b_bluescreen = true;
  i_refreshRatio = 50;
  b_showTrueImage=true;
  


  imageRefreshTimer->start( i_refreshRatio );
  
}

//-----------------------------------------------------------------------

imrec::BlueScreenWid::~BlueScreenWid()
{
  delete constantBackground;
}

//-----------------------------------------------------------------------

void imrec::BlueScreenWid::captureFrame()
{
  
  imrec::Image<RgbColor<uchar> >::iterator j = capturedImage.begin();
  if(b_showTrueImage==true)
  {
    for(imrec::Image<RgbColor<uchar> >::const_iterator i = actualImage.begin();
        i != actualImage.end();i++,j++)
    {
      imrec::convertColor(*i, *j); 
    }
  }
  else
  {
    for(imrec::Image<RgbColor<uchar> >::const_iterator i = cameraImage.begin();i != cameraImage.end();i++,j++)
    {
      imrec::convertColor(*i, *j); 
    }
  }
  
  std::ofstream captured_file(p_filename.toLatin1(), std::ios::app);
  if (!captured_file)
  {
    std::cerr << "Couldn't open file: " << p_filename.toStdString()
              << " for output!" << std::endl;
    return;
  }
  capturedImage.write(captured_file);
  captured_file.close();
}

//-----------------------------------------------------------------------

void imrec::BlueScreenWid::setReference()
{

  imrec::Image<RgbColor<uchar> >::iterator j = referenceImage.begin();
  
  for(imrec::BasicFrameGrabber::const_iterator i = bfg->imageBegin();i != bfg->imageEnd();i++,j++)
  {
    imrec::convertColor(*i,*j);
  }
  
}

//-----------------------------------------------------------------------

void imrec::BlueScreenWid::setTolerance(int tolerance_value)
{
  bluescr.setTolerance(tolerance_value);
}

//-----------------------------------------------------------------------

void imrec::BlueScreenWid::setRefreshRatio(unsigned int time_period)
{
  imageRefreshTimer->setInterval(time_period);
}

//-----------------------------------------------------------------------

void imrec::BlueScreenWid::refreshImage()
{
  if(bfg->isNextFrameAvailable()==true)
  {
    bfg->updateImageData();
    QPainter p;
    QPicture pic;
    p.begin(&pic);
    
    imrec::Image<RgbColor<uchar> >::iterator j = cameraImage.begin();
    for(imrec::BasicFrameGrabber::const_iterator i = bfg->imageBegin();
        i != bfg->imageEnd();i++,j++)
    {
      imrec::convertColor(*i, *j); 
    }
    bfg->releaseImageData();
    
    if (b_bluescreen==true)
    {
      
      bluescr.process(
          cameraImage.begin(),
          cameraImage.end(),
          referenceImage.begin(),
          *constantBackground,
          actualImage.begin());

      for(std::vector<imrec::ImageButton*>::const_iterator button_iterator = vecimbutt.begin(); 
          button_iterator!=vecimbutt.end();button_iterator++)
      {
        (**button_iterator).process();
        (**button_iterator).setmaskcolor(background_color);
        
        if (b_showTrueImage==true)
        {
          (**button_iterator).repaint(p,cameraImage);
        }
        else
        {
          (**button_iterator).repaint(p,actualImage);
        }
        
      }
      if(b_showTrueImage==true)
      {
        setImage(cameraImage.begin(),cameraImage.end());
      }
      else
      {
        setImage(actualImage.begin(),actualImage.end());
      }
      
    }
    else
    {
      setImage(cameraImage.begin(),cameraImage.end());
    }
    p.end();
    setOverlayGraphics(pic);
  }
  
}

//-----------------------------------------------------------------------

void imrec::BlueScreenWid::toggleBluescreen(bool on)
{
  b_bluescreen=on;
}

//-----------------------------------------------------------------------

void imrec::BlueScreenWid::setColor(QColor color)
{  
  background_color = color;
  (**constantBackground).set(color.red(),color.green(),color.blue());
}

//-----------------------------------------------------------------------

void imrec::BlueScreenWid::setFilename(const QString& filename)
{
  p_filename = filename;
}

//-----------------------------------------------------------------------

QColor imrec::BlueScreenWid::getColor()
{
  return (QColor((**constantBackground).red(),(**constantBackground).green(),(**constantBackground).blue()));
}

//-----------------------------------------------------------------------

void imrec::BlueScreenWid::setFileReference(const QString& filereference)
{
  p_filereference = filereference;
  std::ifstream openfile(p_filereference.toLatin1());
  referenceImage.read(openfile);
  openfile.close();
}

//-----------------------------------------------------------------------

void imrec::BlueScreenWid::addButton(imrec::ImageButton *imbutt)
{
  vecimbutt.push_back(imbutt);
  imbutt->setImage(&actualImage);
}

//-----------------------------------------------------------------------

int imrec::BlueScreenWid::numberOfButtons()
{
  return vecimbutt.size();
}

//-----------------------------------------------------------------------
  
void imrec::BlueScreenWid::toggleViewMode(bool view_on)
{
  b_showTrueImage = !view_on;
}



