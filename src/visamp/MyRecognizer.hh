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

#ifndef MYRECOGNIZER_HH
#define MYRECOGNIZER_HH

#include <qobject.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qstring.h>

#include <Recognizer.hh>
#include <Image.hh>
#include <RgbColor.hh>
#include <Sample.hh>
#include <MinimumDistance.hh>

typedef unsigned int uint;
typedef imrec::Image<imrec::RgbColor<uchar> > rgbImage;
typedef imrec::MinimumDistance<imrec::Sample> minDist ;

class MyRecognizer : public QObject, public imrec::Recognizer<rgbImage, minDist>
{
    Q_OBJECT

public:

    MyRecognizer( rgbImage& image, uint top, uint left, uint nrows, uint ncols, const imrec::RgbColor<uchar> mask,
                  float tolerance=0.5, uint hold=5, uint bins=4, bool normalizeSamples=false) 
        : QObject(), imrec::Recognizer<rgbImage, minDist>
    ( image, top, left, nrows, ncols, mask, tolerance, hold, bins, normalizeSamples), lastIndex(-1)
        {} 

    void repaint( QPainter& p, rgbImage& image) const;

    void recognized();

signals:

    void recognizedSig();
    void recChanged();
    void recIndex( int  );
    void recName ( const QString &);

private:

    int lastIndex;
    
};

#endif

