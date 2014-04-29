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

#ifndef SUBIMAGEITERATOR_HH
#define SUBIMAGEITERATOR_HH 

#include <iterator>

#include "SubImage.hh"

namespace imrec
{

/*======================================================================*/
/*!
 *  \class SubImageIterator SubImageIterator.hh
 *  \brief The SubImageIterator template class is an iterator specially
 *         designed for use with the SubImage class.
 */
/*======================================================================*/
template <class SubImageType>
class SubImageIterator: public std::iterator<std::forward_iterator_tag,
                                             typename SubImageType::value_type,
                                             int>
{       
public:

  typedef typename std::iterator<std::forward_iterator_tag,
                                 typename SubImageType::value_type,
                                 int>::reference reference;
  
  /*====================================================================*/
  /*! 
   *   constructor
   *
   *   \param subimage pointer to SubImage
   *   \param row      initial vertical position in subimage
   *   \param position iterator of the base image of the subimage 
   */
  /*====================================================================*/  
  SubImageIterator(SubImageType* subimage, 
                   int row, 
                   typename SubImageType::base_iterator position);


  /*====================================================================*/
  /*! 
   *   prefix operator++. advance the iterator
   *
   *   \return reference to incremented SubImageIterator
   */
  /*====================================================================*/  
  SubImageIterator<SubImageType>& operator++();


  /*====================================================================*/
  /*! 
   *   postfix operator++. advance the iterator
   *
   *   \return reference to unincremented SubImageIterator
   */
  /*====================================================================*/
  SubImageIterator<SubImageType> operator++(int);

  
  /*====================================================================*/
  /*! 
   *   operator==. check equality
   *
   *   \param iter2  iterator whose equality is to be checked
   *
   *   \return true, if both iterators are equal
   */
  /*====================================================================*/  
  bool operator==(const SubImageIterator<SubImageType>& iter2) const;


  /*====================================================================*/
  /*! 
   *   operator!=. check inequality
   *
   *   \param iter2  iterator whose equality is to be checked
   *
   *   \return true, if both iterators are inequal
   */
  /*====================================================================*/  
  bool operator!=(const SubImageIterator<SubImageType>& iter2) const;  


  /*====================================================================*/
  /*! 
   *   operator->. allows the access of members of the original image
   *   iterator. i.e.: subiter->red()  
   *
   *   \return pointer to original iterator
   */
  /*====================================================================*/
  typename SubImageType::base_iterator operator->() const;
  
          
  /*====================================================================*/
  /*! 
   *   set new position
   *
   *   \param row      new vertical position in subimage
   *   \param position iterator of the base image of the subimage
   */
  /*====================================================================*/
  void set(unsigned int row, 
           typename SubImageType::base_iterator position);
  

  /*====================================================================*/
  /*! 
   *   set new position and subimage
   *
   *   \param subimage pointer to subimage
   *   \param row      new vertical position in subimage
   *   \param position iterator of the base image of the subimage
   */
  /*====================================================================*/
  void set(SubImageType* subimage, 
           unsigned int row, 
           typename SubImageType::base_iterator position);


  /*====================================================================*/
  /*! 
   *   operator*. dereference iterator
   *
   *   \return reference to the original iterator of the base image
   */
  /*====================================================================*/  
  reference operator*() const;
  
protected:
  //pointer to the subimage
  SubImageType* p_subimage;
  //actual row number
  unsigned int p_row;
  //actual position within base image
  typename SubImageType::base_iterator p_position;
};
  
#include "SubImageIterator.icc"  
  
}

#endif //SUBIMAGEITERATOR_HH
