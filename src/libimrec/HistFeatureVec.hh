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

#ifndef HistFeatureVec_HH
#define HistFeatureVec_HH

#include <vector>
#include "types.hh"
#include "RgbColor.hh"

namespace imrec
{

/*======================================================================*/
/*!
 *  \class HistFeatureVec HistFeatureVec.hh
 *  \brief The HistFeatureVec class contains a Histogramm feature 
 *         vector out of a image of variable type , 
 *         which saves the number of Rgb(red,green,blue) in 
 *         for example hist[red][gree][blue].
 *
 *
 */
/*======================================================================*/


  class HistFeatureVec
  {
  public:

/*-------------------------------------------------------------------------
 * Iterators and References 
 *-------------------------------------------------------------------------*/
    
    typedef std::vector<int>::iterator        iterator;
    typedef std::vector<int>::const_iterator  const_iterator;
    typedef std::vector<int>::reference       reference;
    typedef std::vector<int>::const_reference const_reference;
    typedef std::vector<int>::size_type       size_type;
  
/*======================================================================*/
/*! 
 *   Constructor 
 *
 *   \param bins number of bins
 *   \param mask mask color
 */
/*======================================================================*/
    HistFeatureVec(
        int bins=4, 
        imrec::RgbColor<unsigned char> mask=
        imrec::RgbColor<unsigned char>(255,0,255));

/*======================================================================*/
/*! 
 *   Default Destructor
 */
/*======================================================================*/

    ~HistFeatureVec();
  
/*======================================================================*/
/*! 
 *   Creates the histogram of an image
 *
 *   \param img Reference to the image of which a histogram
 *              should be extracted
 */
/*======================================================================*/

    template<class ImageType>
    void extract(const ImageType& img);
  
/*======================================================================*/
/*! 
 *   Resizes the histogram vectors as necessary according to p_bin
 */
/*======================================================================*/

    void resize();

/*======================================================================*/
/*! 
 *   Begin of histogram
 *
 *   \return pointer to first RGBColor of image
 */
/*======================================================================*/
  
    iterator       begin();
    const_iterator begin() const;

/*======================================================================*/
/*! 
 *   End of histogram
 *
 *   \return Pointer to the first element BEHIND the last element 
 *           of the histogram
 */
/*======================================================================*/
    
    iterator       end();
    const_iterator end() const;

  /*======================================================================*/
  /*! 
   *   Operator[] 
   *
   *   \param n  histogram element number
   *
   *   \return Reference to element n of the histogram
   */
  /*======================================================================*/
    
    reference       operator[]( size_type n); 
    const_reference operator[]( size_type n) const;


/*======================================================================*/
/*! 
 *   size of the histogram
 *
 *   \return size, which has size_type
 */
/*======================================================================*/
    
    size_type size() const;

  private:
    imrec::RgbColor<unsigned char> p_mask;      //mask colour will NOT be added
    unsigned int p_bin;                         // number of bins
    unsigned int p_size;
    std::vector<int> data;                           //saves all data
    std::vector<iterator> ystart;                    //pointer on data, which mark ystarts
    std::vector<std::vector<iterator>::iterator > zstart; //pointer on ystart, which are also pointers. They mark the zstart
  };

#include "HistFeatureVec.icc"
}

#endif //HISTFEATUREVEC_HH
