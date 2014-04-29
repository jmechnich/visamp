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

#ifndef PLAYER_HH
#define PLAYER_HH

#include <qobject.h>
#include <qtimer.h>
#include <qstring.h>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

#include "MyError.hh"

namespace imrec
{
/*======================================================================*/
/*!
 *  \class Player Player.hh
 *  \brief The Player class provides methods to play MP3 files using a 
 *         using an already installed MP3Player (e.g. mpg123)
 *
 *  The class provides ( being a QObject) full qt functionality with
 *  signals and slots.
 */
/*======================================================================*/


class Player : public QObject
{
    Q_OBJECT
        
public:

/*-----------------------------------------------------------------------
 *  Error classes
 *-----------------------------------------------------------------------*/
    class ReadError  : public MyError { public: ReadError ( std::string s): MyError(s){}};
    class WriteError : public MyError { public: WriteError( std::string s): MyError(s){}};
    
/*-----------------------------------------------------------------------
 *  Iterators and references
 *-----------------------------------------------------------------------*/
    typedef std::vector<QString>::iterator        iterator;
    typedef std::vector<QString>::const_iterator  const_iterator;
    typedef std::vector<QString>::reference       reference;
    typedef std::vector<QString>::const_reference const_reference;
    typedef std::vector<QString>::size_type       size_type;

/*======================================================================*/
/*! 
 *   Constructor with file argument
 *   \param playerpath  path to the mp3-player
 *   \param mp3File     path to a mp3-file or a file playlist
 */
/*======================================================================*/
    Player( QString playerpath, QString filename, QObject * parent=0);
        
/*======================================================================*/
/*! 
 *   Destructor
 */
/*======================================================================*/
    ~Player();

/*======================================================================*/
/*! 
 *   extracts mp3 filenames out of a playlist
 *   \param filename  name of a single mp3 or playlist file
 *   \exception ReadError  thrown if file read error occurs
 *   \return string vector with paths to mp3-files
 */
/*======================================================================*/
    void readFile( QString filename, bool addOnly);

/*======================================================================*/
/*! 
 *   returns filename of currently selected song
 *   \return filename
 */
/*======================================================================*/
    QString getTitle();
        
/*======================================================================*/
/*! 
 *   returns playlist position of currently selected song
 *   \return position
 */
/*======================================================================*/
    unsigned int getCurrent();
        
/*======================================================================*/
/*! 
 *   returns volume
 *   \return volume Range: 0-100 (percent)
 */
/*======================================================================*/
    unsigned int getVolume();
        
/*======================================================================*/
/*! 
 *   begin
 *   \return pointer to the beginning of the playlist vector
 */
/*======================================================================*/
    iterator begin();
    const_iterator begin() const;
  
/*======================================================================*/
/*! 
 *   end
 *   \return pointer to the end of the playlist vector
 */
/*======================================================================*/
    iterator end();
    const_iterator end() const;

/*======================================================================*/
/*! 
 *   size 
 *   \return size of the playlist vector
 */
/*======================================================================*/
    size_type size() const;
  
/*======================================================================*/
/*! 
 *   operator []
 *   \return reference to element number n of the playlist vector
 */
/*======================================================================*/
    reference operator[]( size_type n);
    const_reference operator[]( size_type n) const;

public slots:

/*======================================================================*/
/*! 
 *   play function to play current playlist item
 */
/*======================================================================*/
    void play();

/*======================================================================*/
/*! 
 *   play function to play a specific playlist item
 *   \param pos  playlist position
 */
/*======================================================================*/
    void play( int pos);

/*======================================================================*/
/*! 
 *   stop function
 */
/*======================================================================*/
    void stop();

/*======================================================================*/
/*! 
 *   pause function
 */
/*======================================================================*/
    void pause();

/*======================================================================*/
/*! 
 *   next function
 */
/*======================================================================*/
    void next();
        
/*======================================================================*/
/*! 
 *   previous function
 */
/*======================================================================*/
    void previous();
        
/*======================================================================*/
/*! 
 *   Adds a single mp3 or a file playlist to the internal playlist
 *   \exception ReadError  thrown if file read error occurs
 *   \param filename
 */
/*======================================================================*/
    void addFile( QString filename);
        
/*======================================================================*/
/*! 
 *   sets volume
 *   \param volume  Range: 0-100 (percent)
 */
/*======================================================================*/
    void setVolume( int volume);

/*======================================================================*/
/*! 
 *   mutes or unmutes the mixer
 */
/*======================================================================*/
    void mute();
        
/*======================================================================*/
/*! 
 *   write internal playlist to file
 *   \exception WriteError  thrown if unable to save playlist to disk
 *   \param filename
 */
/*======================================================================*/
    void saveList(QString filename);

private slots:

/*======================================================================*/
/*! 
 *   checks if the player process has terminated
 */
/*======================================================================*/
   void checkPlayer();
    
signals:

/*======================================================================*/
/*! 
 *   signal for current playlist position
 */
/*======================================================================*/
    void currentIndex( int);

/*======================================================================*/
/*! 
 *   signal for current title path
 */
/*======================================================================*/
    void currentTitle( const QString &);

/*======================================================================*/
/*! 
 *   signal for playlist change
 */
/*======================================================================*/
    void playlistChanged();
    
private:
    
    QString p_playerpath;       // path to mp3-player
    int p_pid;                  // process ID of test mp3Player
    unsigned int p_volume;      // output volume
    std::vector<QString> p_playlist; // mp3-filename
    size_type p_current;        // position of current element of playlist
    bool p_muted;               // states if the player is muted
    bool p_paused;              // states if the player is paused
    bool p_stopped;             // states if the stop function was called

    QTimer* timer;              // Timer for checkPlayer() slot
};
#include "Player.icc"
    
}
#endif //PLAYER_HH
