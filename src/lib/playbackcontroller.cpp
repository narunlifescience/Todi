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

#include "playbackcontroller.h"

#include "mpdsocket.h"

// https://www.musicpd.org/doc/protocol/playback_commands.html
const QByteArray PlaybackController::nextCmd = "next";
const QByteArray PlaybackController::pauseCmd = "pause";
const QByteArray PlaybackController::playCmd = "play";
const QByteArray PlaybackController::playIdCmd = "playid";
const QByteArray PlaybackController::previousCmd = "previous";
const QByteArray PlaybackController::seekCmd = "seek";
const QByteArray PlaybackController::seekIdCmd = "seekid";
const QByteArray PlaybackController::seekCurCmd = "seekcur";
const QByteArray PlaybackController::stopCmd = "stop";

PlaybackController::PlaybackController(QObject *parent,
                                       std::shared_ptr<MPDSocket> mpdSocket)
    : QObject(parent), mpdSocket_(mpdSocket) {}

PlaybackController::~PlaybackController() {}

bool PlaybackController::next() const {
  return mpdSocket_->sendCommand(nextCmd).second;
}

bool PlaybackController::pause(quint8 toggle) const {
  return mpdSocket_->sendCommand(pauseCmd + " " + QByteArray::number(toggle))
      .second;
}

bool PlaybackController::play(quint32 song) {
  return mpdSocket_->sendCommand(playCmd + " " + QByteArray::number(song))
      .second;
}

bool PlaybackController::playId(quint32 songid) {
  return mpdSocket_->sendCommand(playIdCmd + " " + QByteArray::number(songid))
      .second;
}

bool PlaybackController::previous() {
  return mpdSocket_->sendCommand(previousCmd).second;
}

bool PlaybackController::seek(quint32 song, quint32 time) {
  return mpdSocket_
      ->sendCommand(seekCmd + " " + QByteArray::number(song) + " " +
                    QByteArray::number(time))
      .second;
}

bool PlaybackController::seekId(quint32 songid, quint32 time) {
  return mpdSocket_
      ->sendCommand(seekIdCmd + " " + QByteArray::number(songid) + " " +
                    QByteArray::number(time))
      .second;
}

bool PlaybackController::seekCur(quint32 time) {
  return mpdSocket_->sendCommand(seekCurCmd + " " + QByteArray::number(time))
      .second;
}

bool PlaybackController::stop() {
  return mpdSocket_->sendCommand(stopCmd).second;
}
