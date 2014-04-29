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

#ifndef SAMPLE_HH
#define SAMPLE_HH

#include <vector>
#include <string>

namespace imrec
{
/*======================================================================*/
/*!
 *  \class Sample Sample.hh 
 *  \brief The Sample class represents a family of feature vectors
 *         of an object, that means the class saves a various number of
 *         FeatVecs and a name and label
 *
 */
/*======================================================================*/


class Sample
{

public:

  typedef std::vector<float>::iterator        iterator;
  typedef std::vector<float>::const_iterator  const_iterator;
  typedef std::vector<float>::reference       reference;
  typedef std::vector<float>::const_reference const_reference;
  typedef std::vector<float>::size_type       size_type;
  
   
/*======================================================================*/
/*! 
 *   Constructor
 *
 *   \param label label of sample
 *   \param name  name of sample
 */
/*======================================================================*/

  Sample(int label=0, std::string name="default");

/*======================================================================*/
/*! 
 *   Destructor
 */
/*======================================================================*/

  ~Sample();
  
/*======================================================================*/
/*! 
 *   getlabel
 *
 *   \return label of sample
 */
/*======================================================================*/

  int getlabel() const;

/*======================================================================*/
/*! 
 *   setlabel
 *
 *   \param label  new sample label
 */
/*======================================================================*/

  void setlabel( int label);
  
/*======================================================================*/
/*! 
 *   getname
 *
 *   \return name of sample
 */
/*======================================================================*/

  std::string getname() const;
  
/*======================================================================*/
/*! 
 *   setname
 *
 *   \param name new sample name
 */
/*======================================================================*/

  void setname( std::string name);
  
/*======================================================================*/
/*!
 *   addFeatVec
 *    adds a BasicFeatureVec, e.g. HistFeatureVec
 *
 *   \param FV_begin  pointer to begin of BasicFeatureVec
 *   \param FV_end    pointer to end of BasicFeatureVec
 */
/*======================================================================*/

template< class FW_iterator>
void addFeatVec( FW_iterator FV_begin, FW_iterator FV_end);
  
/*======================================================================*/
/*! 
 *   begin
 *
 *   \return pointer to the beginning of the sample vector
 */
/*======================================================================*/

  iterator begin();
  const_iterator begin() const;
  
/*======================================================================*/
/*! 
 *   end 
 *
 *   \return pointer to the end of the sample vector
 */
/*======================================================================*/

  iterator end();
  const_iterator end() const;

/*======================================================================*/
/*! 
 *   size
 *
 *   \return size of the sample vector
 */
/*======================================================================*/

  size_type size() const;
  
/*======================================================================*/
/*! 
 *   operator []
 *
 *   \return reference to element number n of the sample vector
 */
/*======================================================================*/
  
  reference operator[]( size_type n);
  const_reference operator[]( size_type n) const;
  

  private:
  int p_label;
  std::string p_name;
  std::vector<float> data; //saves all FeaturesVecs
  int p_number;       // number of included FeatVecs in data

};

#include "Sample.icc"
 
}

#endif //SAMPLE_HH



