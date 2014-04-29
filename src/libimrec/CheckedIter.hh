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

#ifndef CHECKEDITER_HH
#define CHECKEDITER_HH

#include "out_of_bounds.hh"

namespace imrec
{
  template<class Cont, class Iter = typename Cont::iterator>
  class CheckedIter;
  
  template<class Cont, class Iter>
  bool operator==(const CheckedIter<Cont, Iter>& i,
                  const CheckedIter<Cont, Iter>& j);
  
  template<class Cont, class Iter>
  bool operator!=(const CheckedIter<Cont, Iter>& i,
                  const CheckedIter<Cont, Iter>& j);
  

/*======================================================================*/
/*!
 *  \class CheckedIter CheckedIter.hh
 *  \brief The CheckedIter class provides methods to provide checked access to its container
 */
/*======================================================================*/
template<class Cont, class Iter>
class CheckedIter : public iterator_traits<Iter> 
{
  
public:
  typedef typename iterator_traits<Iter>::reference       reference;
  typedef typename iterator_traits<Iter>::pointer         pointer;
  typedef typename iterator_traits<Iter>::difference_type difference_type;
  
  /*-----------------------------------------------------------------------
   *  Error class
   *-----------------------------------------------------------------------*/
class out_of_bounds{ };

 /*======================================================================*/
  /*! 
   *   the valid method checks if the given Iter is valid
   *
   *   \param p  
   */
  /*======================================================================*/ 
  void valid(Iter p);

  friend
  bool operator==<Cont,Iter>(const CheckedIter<Cont, Iter>& i, const CheckedIter<Cont, Iter>& j);
  friend 
  bool operator!=<Cont,Iter>(const CheckedIter<Cont, Iter>& i, const CheckedIter<Cont, Iter>& j);
 
 /*======================================================================*/
  /*! 
   *    Default copy constructor and copy assignment
   */
  /*======================================================================*/ 
  inline CheckedIter(Cont& x, Iter p);
  
 
  reference operator*();

  reference operator[](int d);

  pointer operator->();

  CheckedIter operator+(int d);

  CheckedIter operator-(int d);

  CheckedIter& operator++();
  
  CheckedIter operator++(int);

  CheckedIter& operator-- ();

  CheckedIter operator--(int);

 /*======================================================================*/
  /*! 
   *  gives the distance of the current iterator relative to the begin
   *  of the container.
   *  
   *  return\ the distance
   */
  /*======================================================================*/ 
  
  difference_type index ();
   /*======================================================================*/
  /*! 
   *  gives the unchecked Iterator back
   *  
   *  return\ unchecked Iter
   */
  /*======================================================================*/ 
  Iter unchecked ();

protected:
  Iter akt;
  Cont* c;
  
  
};
 
#include "CheckedIter.icc"
 
}

#endif //CHECKEDITER_HH




