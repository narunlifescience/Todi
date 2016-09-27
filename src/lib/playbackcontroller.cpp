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

// https://www.musicpd.org/doc/protocol/playback_commands.html
const QByteArray PlaybackController::nextCommand = "next";
const QByteArray PlaybackController::pauseCommand = "pause";
const QByteArray PlaybackController::playCommand = "play";
const QByteArray PlaybackController::platIdCommand = "playid";
const QByteArray PlaybackController::previousCommand = "previous";
const QByteArray PlaybackController::seekCommand = "seek";
const QByteArray PlaybackController::seekIdCommand = "seekid";
const QByteArray PlaybackController::seekCurCommand = "seekcur";
const QByteArray PlaybackController::stopCommand = "stop";

PlaybackController::PlaybackController(
    QObject *parent, std::shared_ptr<CommandController> commandctrlr)
    : QObject(parent), cmdCtrlr_(commandctrlr) {}

PlaybackController::~PlaybackController() {}

void PlaybackController::next() const { cmdCtrlr_->sendCommand(nextCommand); }

void PlaybackController::pause(quint8 toggle) const {
  cmdCtrlr_->sendCommand(pauseCommand + " " + static_cast<const char>(toggle));
}

void PlaybackController::play() {}

void PlaybackController::playId() {}

void PlaybackController::previous() { cmdCtrlr_->sendCommand(previousCommand); }

void PlaybackController::seek() {}

void PlaybackController::seekId() {}

void PlaybackController::seekCur() {}

void PlaybackController::stop() { cmdCtrlr_->sendCommand(stopCommand); }
