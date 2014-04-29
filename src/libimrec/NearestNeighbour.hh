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

#ifndef NEARESTNEIGHBOUR_HH
#define NEARESTNEIGHBOUR_HH

//STD
#include <vector>
#include <cmath> //for sqrt

//LIBIMREC
#include "MyError.hh"

namespace imrec
{

/*======================================================================*/
/*!
 *  \class NearestNeighbour NearestNeighbour.hh
 *  \brief The NearestNeighbour class is a Classifier that performs
 *         the nearest neighbour classification.
 *
 *  The NearestNeighbour classifier provides functions to ...
 *    ... search a sample of its sample library with minimum distance to 
 *        a given sample where this distance must be lower than a given
 *        tolerance value
 *    ... add (learn) a given sample to the internal sample library     
 */
/*======================================================================*/  
template <class SampleType>
class NearestNeighbour
{
public:

  /*--------------------------------------------------------------------
   *  Error class 
   *--------------------------------------------------------------------*/
  class SizeError:public MyError{public:SizeError(std::string s):MyError(s){}};


  /*====================================================================*/
  /*! 
   *   Default constructor. 
   *
   *   \param normalizeSamples set TRUE if you want new samples to be 
   *          normalized before being added to the sample library
   */
  /*====================================================================*/
  NearestNeighbour(
      bool normalizeSamples=false);


  /*====================================================================*/
  /*! 
   *   Constructor. Begin and end specify a vector of samples that should 
   *   be added to the sample library. 
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
  NearestNeighbour(
      const typename std::vector<SampleType>::const_iterator begin,
      const typename std::vector<SampleType>::const_iterator end,
      bool normalizeSamples=false);
    
  
  /*====================================================================*/
  /*! 
   *   Classify given sample. The minimum distance of this sample to the 
   *   sample library must be lower than the tolerance value.
   *
   *   \param sample that is to be classified
   *
   *   \exception SizeError the given sample does not have the size of
   *                        the samples in the sample library.
   *
   *   \return class number of matched sample, -1 if no sample matched.
   */
  /*====================================================================*/
  int classify(const SampleType& sample);


  /*====================================================================*/
  /*! 
   *   Classify given feature vector. Does the same as the 
   *   classify(const SampleType& sample) function, but takes only a 
   *   feature vector.
   *
   *   \param begin iterator to the first element of the feature vector
   *   \param end   iterator to the element BEHIND the last element of 
   *                the feature vector
   *
   *   \exception SizeError the given feature does not have the size of
   *                        the features in the sample library.
   *
   *   \return class number of matched sample, -1 if no sample matched.   
   */
  /*====================================================================*/
  template <class IteratorType>
  int classify(
      const IteratorType begin,
      const IteratorType end);
  

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
   *   return the distance of the last classification.
   *
   *   \return distance
   */
  /*====================================================================*/
  float getLastDistance() const;
  

  /*====================================================================*/
  /*! 
   *   set tolerance value 
   *
   *   \param value
   */
  /*====================================================================*/
  void setTolerance(const float value);
  

protected:
  /*====================================================================*/
  /*! 
   *   calculate euklidian distance between two given vectors. The two
   *   vectors must hav the same size.
   *
   *   \param begin1 begin iterator of first vector
   *   \param end1   end iterator of first vector
   *   \param begin2 begin iterator of second vector
   *
   *   \return distance
   */
  /*====================================================================*/
  float euklidianDistance(
      const std::vector<float>::const_iterator begin1, 
      const std::vector<float>::const_iterator end1,
      const typename SampleType::const_iterator begin2) const;
  

  /*====================================================================*/
  /*! 
   *   normalize each featurevector in sampleLib 
   */
  /*====================================================================*/
  void normalize();


  /*====================================================================*/
  /*! 
   *   normalize the given featurevector
   *
   *   \param begin iterator to the begin of the featurevector
   *   \param end   iterator to the end of the featurevector 
   *
   */
  /*====================================================================*/
  void normalize(
      typename SampleType::iterator begin,
      typename SampleType::iterator end);
  
  
  std::vector<SampleType> p_sampleLib;
  float p_minDist;
  float p_tolerance;
  unsigned int p_size;
  bool p_normalize;
};

#include "NearestNeighbour.icc"
 
}

#endif //NEARESTNEIGHBOUR_HH
