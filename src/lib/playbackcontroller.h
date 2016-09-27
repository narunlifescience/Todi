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

#include <commandcontroller.h>
#include <QObject>

class PlaybackController : QObject {
  Q_OBJECT
 public:
  explicit PlaybackController(
      QObject *parent = nullptr,
      std::shared_ptr<CommandController> commandctrlr = nullptr);
  ~PlaybackController();
 public slots:
  void next() const;
  void pause(quint8 toggle) const;
  void play();
  void playId();
  void previous();
  void seek();
  void seekId();
  void seekCur();
  void stop();

 private:
  std::shared_ptr<CommandController> cmdCtrlr_;
  const static QByteArray nextCommand;
  const static QByteArray pauseCommand;
  const static QByteArray playCommand;
  const static QByteArray platIdCommand;
  const static QByteArray previousCommand;
  const static QByteArray seekCommand;
  const static QByteArray seekIdCommand;
  const static QByteArray seekCurCommand;
  const static QByteArray stopCommand;
};

#endif  // PLAYBACKCONTROLLER_H
