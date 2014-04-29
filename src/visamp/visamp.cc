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

#include <iostream>
#include <QApplication>

#include "VisAmpWid.hh"

using namespace std;

int main( int argc, char** argv)
{
  if( argc == 1)
  {
    cout << "usage: visamp playerpath (filename)" << endl;
    return 0;
  }

  try
  {
    /*-------------------------------------------------------------------
     *  Init Qt
     *-------------------------------------------------------------------*/
    QApplication app( argc, argv);
    
    /*-------------------------------------------------------------------
     *  create the main widget
     *-------------------------------------------------------------------*/
    VisAmpWid wid;
    wid.show();
    return app.exec();
  }
  catch( imrec::MyError &err)
  {
    cerr << "error occured: `" << err.msg() << "'\n";
    return 1;
  }
  return 0;
}
