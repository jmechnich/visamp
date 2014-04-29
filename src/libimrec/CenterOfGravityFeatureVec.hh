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

#ifndef CenterOfGravityFeatureVec_HH
#define CenterOfGravityFeatureVec_HH

#include <vector>

namespace imrec
{
  
/*======================================================================*/
/*!
 *  \class CenterOfGravityFeatureVec (CenterOfGravityFeatureVec.hh)
 *  \brief The CenterOfGravityFeatureVec class provides a method to calculate
 *         the center of gravity of an object. The object is specified by a 
 *         color which does not match to the object colors
 */
/*======================================================================*/
  class CenterOfGravityFeatureVec
  {
  public:
 
  /*-------------------------------------------------------------------------
   * iterators and references  
   *-------------------------------------------------------------------------*/
    typedef std::vector<float>::iterator iterator;
    typedef std::vector<float>::const_iterator const_iterator;
    typedef std::vector<float>::reference reference;
    typedef std::vector<float>::const_reference const_reference;
    typedef std::vector<float>::size_type size_type;
    typedef std::vector<float>::value_type value_type;
    

/*======================================================================*/
/*! 
 *   calculates center of gravity of an image
 *
 *   \param image reference to an image  
 *
 *   \param color the color which will be ignored 
 *                (object is not matching this color) 
 *                default color is (1,1,1)
 *
 */
/*======================================================================*/
    template <class ImageType> void extract(const ImageType& image, const typename ImageType::value_type color);


/*======================================================================*/
/*! 
 *   Begin of CenterOfGravityFeatureVec
 *
 *   \return pointer to the first element (float value)
 */
/*======================================================================*/

    iterator begin();
    const_iterator begin() const;

/*======================================================================*/
/*! 
 *   End of the CenterOfGravityFeatureVec
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
 *   size of the CenterOfGravityFeatureVec
 *
 *   \return
 */
/*======================================================================*/

    size_type size() const;



  private:
    std::vector<float> cogvector;
    
  
  };

#include "CenterOfGravityFeatureVec.icc"
}

#endif //CENTEROFGRAVITYFEATUREVEC_HH






