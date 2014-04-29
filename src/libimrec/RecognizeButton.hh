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

#ifndef RECOGNIZEBUTTON_HH
#define RECOGNIZEBUTTON_HH


//LIBIMREC
#include "PixelCountFeatureVec.hh"
#include "SubImage.hh"

namespace imrec
{
/*======================================================================*/
/*!
 *  \class RecognizeButton (recognizeButton.hh)
 *  \brief The RecognizeButton class contains method to
 *
 *  (description)
 */
/*======================================================================*/
  template <class ImageType>
  class RecognizeButton
  {
   
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

   RecognizeButton(unsigned int xpos, unsigned int ypos,
                   unsigned int ncols, unsigned int nrows,
                   const char * button_name=0);
    

/*======================================================================*/
/*! 
 *   Destructor
 */
/*======================================================================*/

    virtual ~RecognizeButton();

    
/*======================================================================*/
/*! 
 *   Process the actual Image
 */
/*======================================================================*/

    void process();
    

/*======================================================================*/
/*! 
 *   Update the internal Subimage
 *
 *   \param im reference to an Image
 *
 */
/*======================================================================*/
    
    void setImage(ImageType* im);

/*======================================================================*/
/*! 
 *   Set the mask color 
 *
 *   \param color RgbColor which will be the mask color
 *
 */
/*======================================================================*/

    void setmaskcolor(typename ImageType::value_type maskcolor);


/*======================================================================*/
/*! 
 *   This method sets threshold (number of pixels different from maskcolor)
 *   of activation
 *
 *   \param level Number of pixel that have to be different from maskcolor
 *
 */
/*======================================================================*/

    void bsetActivationThreshold(unsigned int level);


/*======================================================================*/
/*! 
 *   This method sets threshold (number of pixels different from maskcolor)
 *   of deactivation
 *
 *   \param level Number of pixel that have to be different from maskcolor
 *
 */
/*======================================================================*/

    void bsetDeactivationThreshold(unsigned int level);
    


    virtual void bclicked();
    virtual void bpressed();
    virtual void breleased();
    

  protected:
    imrec::SubImage<ImageType > *image;
    unsigned int p_nrows;
    unsigned int p_ncols;
    unsigned int p_xpos;
    unsigned int p_ypos;
    const char * p_button_name;
    unsigned int i_activation_threshold;
    unsigned int i_deactivation_threshold;


  private:
    PixelCountFeatureVec pixelcount;

    
    unsigned int i_activated; //integer value to check if activation holds
    //over a sequence of frames
    bool b_clicked; //boolean value to check internal button state
    typename ImageType::value_type p_maskcolor;
    
  };
  
  
#include "RecognizeButton.icc"
  
}

#endif //RECOGNIZEBUTTON_HH

















