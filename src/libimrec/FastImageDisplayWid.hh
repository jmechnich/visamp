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

#ifndef FASTIMAGEDISPLAYWID_HH
#define FASTIMAGEDISPLAYWID_HH

#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <sstream>

#include "MyError.hh"
#include "types.hh"
#include "convertColor.hh"

namespace imrec
{

/*======================================================================*/
/*!
 *  \class FastImageDisplayWid FastImageDisplayWid.hh
 *  \brief The FastImageDisplayWid class is a basic visualization 
 *         class for displaying image sequences.
 *
 */
/*======================================================================*/

  class FastImageDisplayWid : public QWidget
  {
    Q_OBJECT
  public:
    /*-----------------------------------------------------------------------
     *  Error classes
     *-----------------------------------------------------------------------*/
    class UnsupportedBitDepth: public MyError
    { public: UnsupportedBitDepth( std::string s) : MyError(s) {}};  
    
    
    /*======================================================================*/
    /*! 
     *   Constructor 
     *
     *   \param parent   parent QT-Widget 
     *   \param name     name of Widget
     *   \param f        Drawing-flags
     *
     */
    /*======================================================================*/
    FastImageDisplayWid( QWidget * parent=0, Qt::WindowFlags f = 0); //WRepaintNoErase );
    
    
    /*======================================================================*/
    /*! 
     *   set size of image to be displayed
     *
     *   \param nrows  number of rows
     *   \param ncols  number of cols
     *
     *   \exception UnsupportedBitDepth
     *         
     */
    /*======================================================================*/
    void setImageSize( uint nrows, uint ncols);

    /*======================================================================*/
    /*! 
     *   template method to set the image for displaying.  
     *
     *   \param begin iterator pointing to the beginning of the image
     *   \param end   iterator pointing to the end of the image
     *
     */
    /*======================================================================*/
    template< class Iterator>
    void setImage( const Iterator& begin, const Iterator& end);
    
    /*======================================================================*/
    /*! 
     *   overloaded sizeHint returns recommended size for this Widget
     *
     *   \return recommended size for this Widget
     */
    /*======================================================================*/
    QSize sizeHint() const;
    

  protected:
    virtual void paintEvent( QPaintEvent * );
    
    
  private:
    GC     imageGC;  // graphic context for the image
    XImage *xImage;  // image converted to actual screeen bit depth for redrawing
  };


#include "FastImageDisplayWid.icc"
}

#endif //FASTIMAGEDISPLAYWID_HH



