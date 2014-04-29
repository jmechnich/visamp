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

#ifndef PeripheryFeatureVec_HH
#define PeripheryFeatureVec_HH

#include <Image.hh>
#include <iostream.h>

#include "types.hh"
#include "RgbColor.hh"

namespace imrec
{
/*======================================================================*/
/*!
 *  \class PeripheryFeatureVec PeripheryFeatureVec.hh
 *  \brief PeripheryFeatureVec class
 *
 */
/*======================================================================*/  
template<class ImageType> 
class PeripheryFeatureVec
      {
  public:
        
/*======================================================================*/
/*! 
 *   Constructor      
 */
/*======================================================================*/
    PeripheryFeatureVec();


/*=======================================================================*/
/*
 *  \brief  the extract method computes the amount of periphery pixels
 *          of an Object given by an image and a backgroundcolor.
 *  
 *  \param     image   this is the image which holds the Object.
 *  \param     color   color of the background
 *  
 *  \return    int     the amount of periphery pixels
 */
/*=======================================================================*/
     int extract(const ImageType* image, const typename ImageType::value_type color );
     
/*=======================================================================*/
/*   
 *  \brief  the 'setTolerance' method sets the number maximum neighbours a
 *          peripheral pixel may have
 *   
 *  \param  tol  number of maximum neighbours a peripheral pixel may have
 */  
/*=======================================================================*/
        void setTolerance(unsigned int tol);
     
/*======================================================================*/
/*! 
 *   pointer to the first and only element
 *
 *   \return pointer to the first and only element (int value)
 */
/*======================================================================*/   
        int* begin();
        const int* begin() const;
        
/*======================================================================*/
/*! 
 *   End of the FeatureVector
 *
 *   \return reference to the element behind the last and only element
 */
/*======================================================================*/

    int* end();
    const int* end() const;

/*======================================================================*/
/*! 
 *  operator[]
 *
 *   \param n element number  
 *
 *   \return  reference to the one and only element, no matter what n is  
 */
/*======================================================================*/
    int& operator[](uint n);
    int& operator[](uint n) const;

/*======================================================================*/
/*! 
 *   size of the PeripheryFeatureVec - feature
 *
 *   \return  amount of data elements representing this feature. (=1) 
 */
/*======================================================================*/
    uint size() const;

  protected:
        typename ImageType::value_type BackgroundColor;
        float tolerance;
        float peripheralPixels;
        
  };
  
#include "PeripheryFeatureVec.icc"
}

#endif //PERIPHERYFEATUREVEC_HH











