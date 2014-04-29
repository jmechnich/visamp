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

#ifndef MYERROR_HH
#define MYERROR_HH

#include <string>

namespace imrec 
{
  
/*======================================================================*/
/*!
 *  \class MyError MyError.hh
 *  \brief The MyError class is the base class for all errors in this
 *  library.
 *
 *  The inherited classes are used for throwing, so that the library
 *  user can do a simple catch( MyError err) to catch all errors.
 */
/*======================================================================*/
class MyError
{
public: 
  /*======================================================================*/
  /*! 
   *   Constructor taking a plain text error message
   *
   *   \param s  plain text error message
   */
  /*======================================================================*/
  MyError( std::string s)
          : message(s)
        {
          // nothing to do here
        }

  /*======================================================================*/
  /*! 
   *   return the plain text error message, that was passed to the
   *   constructor  
   *
   *   \return plain text error message
   */
  /*======================================================================*/
  const std::string &msg() const
        {
          return message;
        }
  

private:  
  std::string message; 
};
 
}

#endif //MYERROR_HH

