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

#ifndef MINIMUMDISTANCE_HH
#define MINIMUMDISTANCE_HH

//STD
#include <map>

//LOCAL
#include "NearestNeighbour.hh"
#include "MyError.hh"

namespace imrec
{

/*======================================================================*/
/*!
 *  \class MinimumDistance MinimumDistance.hh
 *  \brief The MinimumDistance class is a Classifier that performs
 *         the minimum distance classification.
 *
 *  The difference between NearestNeighbour and MinimumDistance is, that
 *  the MinimumDistance classifier calculates the average sample of all
 *  samples with the same ID. 
 *  The MinimumDistance classifier provides functions to ...
 *    ... search a sample of its sample library with minimum distance to 
 *        a given sample where this distance must be lower than a given
 *        tolerance value
 *    ... add (learn) a given sample to the internal sample library
 */
/*======================================================================*/
template <class SampleType>
class MinimumDistance: public imrec::NearestNeighbour<SampleType>
{

  class SizeError : public MyError
  {
  public:
    SizeError( const std::string& s) 
            : MyError( s)
          {}
  };
  
public:
  /*====================================================================*/
  /*! 
   *   Default constructor
   * 
   *   \param normalizeSamples set TRUE if you want new samples to be 
   *          normalized before being added to the sample library
   */
  /*====================================================================*/
  MinimumDistance(
      bool normalizeSamples=false);


  /*====================================================================*/
  /*! 
   *   Constructor
   *
   *   \param begin iterator to the first element of the sample vector
   *   \param end   iterator to the element BEHIND the last element of 
   *                the sample vector
   *   \param normalizeSamples set TRUE if you want new samples to be 
   *          normalized before being added to the sample library
   *
   *   \exception SizeError not all samples in the given vector have the
   *                        same size. 
   */
  /*====================================================================*/
  MinimumDistance(
      const typename std::vector<SampleType>::const_iterator begin,
      const typename std::vector<SampleType>::const_iterator end,
      bool normalizeSamples=false);


  /*====================================================================*/
  /*! 
   *   add sample to sample library
   *
   *   \param sample sample that should be added to the library
   *
   *   \exception SizeError the given sample does not have the size of
   *                        the samples in the sample library.
   */
  /*====================================================================*/
  void learn(SampleType sample);

  
protected:
  //first index: sample id/index
  //second index: number of sample with that id/index
  std::map<int, int> p_count;

  //first index: sample id/index
  //second index: position of sample with that id/index within the sample library 
  std::map<int, int> p_position;
}; 

#include "MinimumDistance.icc"

}


#endif //MINIMUMDISTANCE_HH
