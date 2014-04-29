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

#include <cstdlib>

#include <qfile.h>
#include <qtextstream.h>

#include "Player.hh"

imrec::Player::Player( QString playerpath, QString filename, QObject * parent)
    : QObject( parent)
{
    p_playerpath = playerpath;
    p_muted      = 0;
    p_paused     = 0;
    p_stopped    = 1;
    p_current    = 0;
    p_volume     = 0;
    p_pid        = -1;
    readFile( filename, false);

    timer = new QTimer();
    connect( timer, SIGNAL( timeout()), this, SLOT( checkPlayer()));

    timer->start( 1000);
}

//-----------------------------------------------------------------------

imrec::Player::~Player()
{
    timer->stop();
    delete timer;
}

//-----------------------------------------------------------------------

void imrec::Player::play()
{
    
  if(p_stopped){
    p_paused  = 0;
    p_stopped = 0;
    
    p_pid = fork();
    if(p_pid == 0){
      std::ostringstream path;
      path << p_playerpath.toStdString();
      
      std::ostringstream os;
      os <<  p_playlist[p_current].toStdString();
      execl(path.str().c_str(), path.str().c_str(), os.str().c_str(), NULL);
      std::exit(127);
    }
    
    emit currentIndex( p_current);
    emit currentTitle( p_playlist[p_current]);
  }
  
  
}

//-----------------------------------------------------------------------

void imrec::Player::play( int pos)
{
  if(p_paused)
  {
    pause();
  }

 else
 {
   if( (unsigned int) pos < p_playlist.size())
   {  
     if(!p_stopped){
       stop();
     }
     
     p_current = pos;
     play();
   }  
 }
}

//-----------------------------------------------------------------------

void imrec::Player::next()
{
  if(p_paused)
  {
    pause();
  }
  
  if(p_current < p_playlist.size()-1){
        stop();
        p_current++;
        play();
    }
}

//-----------------------------------------------------------------------

void imrec::Player::previous()
{
  if(p_paused)
  {
    pause();
  }
  
  if(p_current > 0){
        stop();
        p_current--;
        play();
    }
}

//-----------------------------------------------------------------------

void imrec::Player::readFile( QString filename, bool addOnly)
{
    std::vector<QString> tmp;
    
    if( filename.contains( ".mp3") || filename.contains( ".MP3")){   //reads a mp3-file
        tmp.push_back( filename);

    }else{                                    //reads a playlist
        QString path = filename.left(filename.lastIndexOf("/")+1);
        QFile f(filename);
 
        if( f.open(QIODevice::ReadOnly)){
            QTextStream t( &f );
            QString s;
            while ( !t.atEnd() ) {
                s = t.readLine();

                if( s.at( 0) != '#'){
                    if( s.contains( "/")){
                        tmp.push_back( s);

                    } else {
                        std::ostringstream os;
                        os << path.toStdString() << s.toStdString();
                        tmp.push_back( QString(os.str().c_str()));

                    }
                }
            }
            f.close();

        } else {
            std::ostringstream err;
            err << "Error while reading playlist";
            throw ReadError(err.str());
        }
    }

    if( addOnly){
        for( iterator tmp_it = tmp.begin(); tmp_it != tmp.end(); tmp_it++){
            p_playlist.push_back(*tmp_it);
        }
    } else {
        p_playlist = tmp;
        p_current = 0;
    }        
    emit playlistChanged();
}

//-----------------------------------------------------------------------

void imrec::Player::saveList( QString filename)
{

    QFile f( filename);
    if( f.open( QIODevice::WriteOnly)){
        QTextStream t( &f);
    
        for(iterator it= p_playlist.begin();it!=p_playlist.end(); it++)
        { 
            t << (*it) << "\n";
        }
        f.close();
    } else {
        std::ostringstream err;
        err << "Error while writing playlist";
        throw WriteError(err.str());
    }
}

#ifdef SGI_GRABBER

//-----------------------------------------------------------------------

void imrec::Player::setVolume( int volume)
{
    std::ostringstream commandline;
    commandline << "audiopanel -device Default Output -gain " << volume << ".0% -nodisplay";
    int ret = std::system( commandline.str().c_str());
    p_volume = volume;
}

//-----------------------------------------------------------------------

void imrec::Player::mute()
{
  int ret;
  if( p_muted)
  {
    ret = std::system( "audiopanel -device Default Output -unmute -nodisplay");
    p_muted=0;
  }
  else
  {
    ret = std::system( "audiopanel -device Default Output -mute -nodisplay");
    p_muted=1;
  }
}


#else //SGI_GRABBER
#ifdef LINUX_GRABBER

//-----------------------------------------------------------------------

void imrec::Player::setVolume( int volume)
{
    std::ostringstream commandline;
    commandline << "amixer set Master " << volume << ".0%";
    int ret = std::system( commandline.str().c_str());
    p_volume = volume;
}

//-----------------------------------------------------------------------

void imrec::Player::mute()
{
  int ret;
  if( p_muted)
  {
    ret = std::system( "amixer set Master unmute");
    p_muted=0;
  }
  else
  {
    ret = std::system( "amixer set Master mutmute");
    p_muted=1;
  }
}

#else
//-----------------------------------------------------------------------

void imrec::Player::setVolume( int volume)
{
    std::ostringstream commandline;
    commandline << "amixer set Master " << volume << ".0%";
    int ret = std::system( commandline.str().c_str());
    p_volume = volume;
}

//-----------------------------------------------------------------------

void imrec::Player::mute()
{
  int ret;
  if( p_muted)
  {
    ret = std::system( "amixer set Master unmute");
    p_muted=0;
  }
  else
  {
    ret = std::system( "amixer set Master mutmute");
    p_muted=1;
  }
}

#endif //LINUX_GRABBER
#endif //SGI_GRABBER

//-----------------------------------------------------------------------

void imrec::Player::stop()
{
    if(!p_stopped){

        p_stopped = 1;
        kill( p_pid, SIGTERM);
    }
    
}

//-----------------------------------------------------------------------

void imrec::Player::pause()
{
    if( p_paused){
      kill( p_pid, SIGCONT);
      p_paused=0;  
    } else {
        kill( p_pid, SIGSTOP);
        p_paused=1;
    }
}

//-----------------------------------------------------------------------

void imrec::Player::checkPlayer()
{
    if(p_current < p_playlist.size()){
        int pid = waitpid( p_pid, NULL, WNOHANG);

        if( pid == p_pid && !p_stopped){
            next();
        }

    } else {
        p_stopped = 1;
    }
}
