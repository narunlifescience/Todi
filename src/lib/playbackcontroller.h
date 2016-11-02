/* This file is part of Todi.

   Copyright 2016, Arun Narayanankutty <n.arun.lifescience@gmail.com>

   Todi is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   Todi is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with Todi.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include <QObject>
#include <memory>
class MPDSocket;

class PlaybackController : QObject {
  Q_OBJECT
 public:
  explicit PlaybackController(
      QObject *parent = nullptr,
      std::shared_ptr<MPDSocket> mpdSocket = nullptr);
  ~PlaybackController();
 public slots:
  void next() const;
  void pause(quint8 toggle) const;
  void play(quint32 song);
  void playId(quint32 songid);
  void previous();
  void seek(quint32 song, quint32 time);
  void seekId(quint32 songid, quint32 time);
  void seekCur(quint32 time);
  void stop();

 private:
  std::shared_ptr<MPDSocket> mpdSocket_;
  const static QByteArray nextCmd;
  const static QByteArray pauseCmd;
  const static QByteArray playCmd;
  const static QByteArray playIdCmd;
  const static QByteArray previousCmd;
  const static QByteArray seekCmd;
  const static QByteArray seekIdCmd;
  const static QByteArray seekCurCmd;
  const static QByteArray stopCmd;
};

#endif  // PLAYBACKCONTROLLER_H
