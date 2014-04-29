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

#ifndef SLIDER_HH
#define SLIDER_HH

#include <cmath>
#include <math.h>

#include <qpainter.h>
#include <qcolor.h>
#include <qbrush.h>
 
#include "CenterOfGravityFeatureVec.hh"
#include "SubImage.hh"

namespace imrec
{
  
//typedef unsigned int uint;

template<class ImageType>
class Slider
{

public:

/*======================================================================*/
/*! 
 *   Constructor
 *   \param  left   number of first column of the subimage
 *   \param  top    number of first row of the subimage
 *   \param  width  number of columns of the subimage
 *   \param height  number of rows of the subimage
 */
/*======================================================================*/
  Slider(uint left, uint top, uint width, uint height);

/*======================================================================*/
/*! 
 *   Destructor
 */
/*======================================================================*/
  virtual ~Slider();

/*======================================================================*/
/*! 
 *   gives the slider-value, detected at the last call of 'process()'
 */
/*======================================================================*/
  int value();

/*======================================================================*/
/*! 
 *   Eventfunction which is called when the process-method 
 *   detected a changed value. It is triggered by process()
 */
/*======================================================================*/
  virtual void sliderMoved();

/*======================================================================*/
/*! 
 *   calibrates the 'minimum' configuration of the slider
 */
/*======================================================================*/
  void calibrateMin();

/*======================================================================*/
/*! 
 *   calibrates the 'maximum' configuration of the slider
 */
/*======================================================================*/
  void calibrateMax();
  
/*======================================================================*/
/*! 
 *   delivers the (sub-) image where the slider is represented in
 *
 *   \param image  the (sub-) image
 *   \param col    the color of the background which helps to tell
 *                 the object
 */
/*======================================================================*/
  void setImage(ImageType* image, typename ImageType::value_type col);

/*======================================================================*/
/*! 
 *   \brief  the given image is processed to compute the current 
 *           centre of gravity of the slider.
 *           If the new value differ from the old, the new is stored
 *           and sliderMoved is triggered
 */
/*======================================================================*/
  void process();

/*======================================================================*/
/*! 
 *   \brief  adds three blocks to the (sub-) image which
 *   represents the states for Maximum, Minimum and current state
 *
 *   \param p    the painter which is used for painting
 *   \param img  the image to paint on
 */
/*======================================================================*/
  void repaint(QPainter& p, ImageType& img);

/*======================================================================*/
/*! 
 *   sets the minimum value of the slider
 *
 *   \param minval  minimum value of the slider  
 */
/*======================================================================*/  
  void setMinVal(int minval); 

/*======================================================================*/
/*! 
 *   sets the maximum value of the slider
 *
 *   \param maxval  the maximum value of the slider
 */
/*======================================================================*/
  void setMaxVal(int maxval);  

protected:
  ImageType*                       p_image;
  imrec::SubImage<ImageType>*      p_subimage;
  typename ImageType::value_type   p_color;
  imrec::CenterOfGravityFeatureVec cof;

  float p_maxX, p_maxY, p_minX, p_minY, p_minval, p_maxval;
  float p_currentX, p_currentY;
  uint p_top, p_left, p_width, p_height;
  int p_value;
  

};

#include "Slider.icc"
 
}

#endif //SLIDER_HH









