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

#ifndef RECOGNIZER_HH
#define RECOGNIZER_HH

//std
#include <map>

//qt includes
#include <qpainter.h>
#include <qcolor.h>
#include <qfont.h>

//imrec includes
#include "HistFeatureVec.hh"
#include "Sample.hh"
#include "convertColor.hh"
#include "SubImage.hh"
#include "Image.hh"

namespace imrec
{

/*======================================================================*/
/*!
 *  \class Recognizer Recognizer.hh
 *  \brief The Recognizer class provides functions to perform object 
 *  classification within a rectangular area of an image and to add
 *  new images/objects to its sample library.
 *
 *  The Recognizer class operates on a rectangular area within an 
 *  existing image. The dimensions of that area can be set in the 
 *  constructor. The learn function adds new samples to the internal
 *  sample library of the classifier. The process function searches the 
 *  sample with the minimum distance to the actual image, and calls the
 *  virtual event function recognized() if such a sample was found.
 *
 */
/*======================================================================*/
template <class ImageType, class ClassifierType>
class Recognizer
{
public:

  /*======================================================================*/
  /*! 
   *   constructor that sets the sample library. 
   *
   *   \param image     base image
   *   \param top       top coordinate of the area in the image
   *   \param left      left coordinate of the area in the image
   *   \param nrows     height of the area in the image
   *   \param ncols     width of the area in the image 
   *   \param mask      maskcolor of image
   *   \param begin     begin of Sample-library
   *   \param end       end of Sample-library
   *   \param tolerance tolerance for classifier
   *   \param hold      number of needed matches for recognition
   *   \param bins      number of bins for histogram
   *   \param normalizeSample normalize Samples before adding to sample lib
   *
   *   \exception       SizeError (see Classifier)
   */
  /*======================================================================*/
  Recognizer(
      ImageType& image,
      unsigned int top, 
      unsigned int left,
      unsigned int nrows,
      unsigned int ncols,
      const typename ImageType::value_type mask,
      const std::vector<imrec::Sample>::const_iterator begin,
      const std::vector<imrec::Sample>::const_iterator end,
      float tolerance=0.5,
      unsigned int hold=5,
      unsigned int bins=4, 
      bool normalizeSamples=false);
  
  
  /*====================================================================*/
  /*! 
   *   Constructor that does not set the sample library
   *
   *   \param image     base image
   *   \param top       top coordinate of the area in the image
   *   \param left      left coordinate of the area in the image
   *   \param nrows     height of the area in the image
   *   \param ncols     width of the area in the image
   *   \param mask            maskcolor of image
   *   \param tolerance       tolerance for classifier
   *   \param hold            number of needed matches for recognition
   *   \param bins            number of bins for histogram
   *   \param normalizeSample normalize Samples before adding to sample lib
   *
   *   \exception       SizeError (see Classifier)
   */
  /*====================================================================*/  
  Recognizer(
      ImageType& image,
      unsigned int top, 
      unsigned int left,
      unsigned int nrows,
      unsigned int ncols,
      const typename ImageType::value_type mask,
      float tolerance=0.5,
      unsigned int hold=5,
      unsigned int bins=4, 
      bool normalizeSamples=false);


  /*====================================================================*/
  /*! 
   *   Destructor
   */
  /*====================================================================*/ 
  virtual ~Recognizer();


  /*======================================================================*/
  /*! 
   *   process actual image
   *
   *   \exception SizeError (see Classifier.classify)
   */
  /*======================================================================*/
  void process();


  /*====================================================================*/
  /*! 
   *   add given image to the sample library of the classifier
   *
   *   \param image      image to add
   *   \param classIndex index for the sample that is created from 
   *                     the image
   *   \param name       name for the sample
   */
  /*====================================================================*/   
  void learn(
      ImageType& image,
      int classIndex,
      std::string name);


  /*====================================================================*/
  /*! 
   *   return name of sample with given index. return "unknown" if no
   *   sample with this index was found
   *
   *   \param index index of sample
   *
   *   \return name of sample
   */
  /*====================================================================*/  
  std::string getName(int index) const;

  /*====================================================================*/
  /*! 
   *   returns recognized image from the map Library
   *
   *   \param index Index of the recognized image 
   *
   *   \return recognized image
   */
  /*====================================================================*/
  const imrec::Image<typename ImageType::value_type>& 
  getImage(const int index);
  
  
  
  /*====================================================================*/
  /*! 
   *   function to set the image
   *
   *   \param image  image to set
   */
  /*====================================================================*/
  void setImage(const ImageType& image);


  /*====================================================================*/
  /*! 
   *   Shows information about the recognized image within the grabbed
   *   image
   *
   *   \param p     QPainter
   *   \param image Actual image
   *
   */
  /*====================================================================*/  
  void repaint(
      QPainter& p, 
      ImageType& image) const;
  

  /*====================================================================*/
  /*! 
   *   return index of last recognized image
   *
   *   \return index of last recognized image
   */
  /*====================================================================*/  
  int recognizedIndex() const;

  /*====================================================================*/
  /*! 
   *   return distance from last recognition
   *
   *   \return distance from last recognition
   */
  /*====================================================================*/  
  float getLastDistance() const;
  
  /*====================================================================*/
  /*! 
   *   event function that is called when image was recognized
   */
  /*====================================================================*/
  virtual void recognized();


  /*====================================================================*/
  /*! 
   *   function to set tolerance
   *
   *   \param tolerance
   */
  /*====================================================================*/
  void setTolerance(float tolerance);
  

protected:
  imrec::SubImage<ImageType> p_subImage;
  unsigned int p_top;
  unsigned int p_left;
  unsigned int p_nrows;
  unsigned int p_ncols;
  typename ImageType::value_type p_mask;
  ClassifierType classifier;
  std::map<int, imrec::Image<typename ImageType::value_type> > p_imageLib;

  unsigned int p_hold;
  unsigned int p_bins;
  
  int p_index;
  unsigned int hold_count;
  HistFeatureVec hist;
  
};
 
#include "Recognizer.icc" 

}


#endif //RECOGNIZER_HH
