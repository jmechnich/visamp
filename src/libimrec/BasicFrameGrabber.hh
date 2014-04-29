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

#ifndef BASICFRAMEGRABBER_HH
#define BASICFRAMEGRABBER_HH

#ifdef SGI_GRABBER

#include <BasicSGIFrameGrabber.hh>
namespace imrec
{
  typedef BasicSGIFrameGrabber BasicFrameGrabber;
}

#else //SGI_GRABBER

#ifdef LINUX_GRABBER

#include <BasicLinuxFrameGrabber_v4l2.hh> 
namespace imrec
{
  typedef BasicLinuxFrameGrabber BasicFrameGrabber;
}

#else  //LINUX_GRABBER

//#include <BasicDummyFrameGrabber.hh>
#include <FileFrameGrabber.hh>
namespace imrec
{
  //typedef BasicDummyFrameGrabber BasicFrameGrabber;
  typedef FileFrameGrabber BasicFrameGrabber;
}

#endif //LINUX_GRABBER
#endif //SGI_GRABBER
  
#endif //BASICFRAMEGRABBER_HH
