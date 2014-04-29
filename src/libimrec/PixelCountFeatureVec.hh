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

#ifndef PixelCountFeatureVec_HH
#define PixelCountFeatureVec_HH

#include <vector> 
#include <iostream>

namespace imrec
{
  
/*======================================================================*/
/*
 *  \class (PixelCountFeatureVec) (PixelCountFeatureVec.hh)
 *  \brief The (PixelCountFeatureVec) class privides a class to calculate 
 *         the number of pixel not matching the specified color
 *
 *  (description)
 */
/*======================================================================*/
  class PixelCountFeatureVec
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
 *   (description)
 *
 *   \param 
 *
 *   \exception 
 *
 */
/*======================================================================*/
    template <class  ImageType> void extract(const ImageType& image,
                                             const typename ImageType::value_type);

/*======================================================================*/
/*! 
 *   Begin of PixelCountFeatureVec
 *
 *   \return pointer to the first element (float value)
 */
/*======================================================================*/

    iterator begin();
    const_iterator begin() const;

/*======================================================================*/
/*! 
 *   End of the PixelCountFeatureVec
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
 *   size of the PixelCountFeatureVec
 *
 *   \return
 */
/*======================================================================*/
    
    size_type size() const;
    

  private:
    std::vector<float> pcfvector;
  
  };

#include "PixelCountFeatureVec.icc"
}

#endif //PIXELCOUNTFEATUREVEC_HH








