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

#ifndef MeanColorFeatureVec_HH
#define MeanColorFeatureVec_HH

#include <vector>

namespace imrec
{

/*======================================================================*/
/*!
 *  \class MeanColorFeatureVec MeanColorFeatureVec.hh
 *  \brief The MeanColorFeatureVec class provides a method for calculating 
 *         the mean color of an object on homogeneous background. 
 *
 */
/*======================================================================*/

  
  class MeanColorFeatureVec
  {
    
  public:

  /*-------------------------------------------------------------------------
   * iterators and references  
   *-------------------------------------------------------------------------*/
    typedef vector<float>::iterator iterator;
    typedef vector<float>::const_iterator const_iterator;
    typedef vector<float>::reference reference;
    typedef vector<float>::const_reference const_reference;
    typedef vector<float>::size_type size_type;
    typedef vector<float>::value_type value_type;


    
/*======================================================================*/
/*! 
 *   Calculates the mean color of the pixels not matching a spezified
 *   color
 *
 *   \param image reference to an image
 *
 *   \param color the color which will be ignored 
 *                default color is (1,1,1)
 */
/*======================================================================*/
    template <class  ImageType> void extract(const ImageType& image,
                                             const typename ImageType::value_type color);


/*======================================================================*/
/*! 
 *   Begin of MeanColorFeatureVec
 *
 *   \return pointer to the first element (float value)
 */
/*======================================================================*/

    iterator begin();
    const_iterator begin() const;

/*======================================================================*/
/*! 
 *   End of the MeanColorFeatureVec
 *
 *   \return reference to the element behind the last element
 */
/*======================================================================*/

    iterator end();
    const_iterator end() const;
    
/*======================================================================*/
/*! 
 *  operator[]
 *
 *   \param n element number  
 *
 *   \return reference to element number n  
 */
/*======================================================================*/

    reference operator[](size_type n);
    const_reference operator[](size_type n) const;

/*======================================================================*/
/*! 
 *   size of the MeanColorFeatureVec
 *
 *   \return
 */
/*======================================================================*/
    
    size_type size() const;
    
    
  private:
    vector<float> mcfvector;
    
  };

#include "MeanColorFeatureVec.icc"
}

#endif //MEANCOLORFEATUREVEC_HH

































