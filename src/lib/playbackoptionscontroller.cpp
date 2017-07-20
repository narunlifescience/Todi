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

#include "playbackoptionscontroller.h"

#include "mpdsocket.h"

// https://www.musicpd.org/doc/protocol/playback_option_commands.html
const QByteArray PlaybackOptionsController::consumeCmd = "consume";
const QByteArray PlaybackOptionsController::crossfadeCmd = "crossfade";
const QByteArray PlaybackOptionsController::mixrampdbCmd = "mixrampdb";
const QByteArray PlaybackOptionsController::mixrampdelayCmd = "mixrampdelay";
const QByteArray PlaybackOptionsController::randomCmd = "random";
const QByteArray PlaybackOptionsController::repeatCmd = "repeat";
const QByteArray PlaybackOptionsController::setvolCmd = "setvol";
const QByteArray PlaybackOptionsController::singleCmd = "single";
const QByteArray PlaybackOptionsController::replay_gain_modeCmd =
    "replay_gain_mode";
const QByteArray PlaybackOptionsController::replay_gain_statusCmd =
    "replay_gain_status";
const QByteArray PlaybackOptionsController::volumeCmd = "volume";

PlaybackOptionsController::PlaybackOptionsController(
    QObject *parent, std::shared_ptr<MPDSocket> mpdSocket)
    : QObject(parent), mpdSocket_(mpdSocket) {}

PlaybackOptionsController::~PlaybackOptionsController() {}

bool PlaybackOptionsController::consume(quint8 toggle) {
  return mpdSocket_->sendCommand(consumeCmd + " " + QByteArray::number(toggle))
      .second;
}

bool PlaybackOptionsController::crossfade(int seconds) {
  return mpdSocket_
      ->sendCommand(crossfadeCmd + " " + QByteArray::number(seconds))
      .second;
}

bool PlaybackOptionsController::random(quint8 toggle) {
  return mpdSocket_->sendCommand(randomCmd + " " + QByteArray::number(toggle))
      .second;
}

bool PlaybackOptionsController::repeat(quint8 toggle) {
  return mpdSocket_->sendCommand(repeatCmd + " " + QByteArray::number(toggle))
      .second;
}

bool PlaybackOptionsController::setvol(quint8 vol) {
  return mpdSocket_->sendCommand(setvolCmd + " " + QByteArray::number(vol))
      .second;
}

bool PlaybackOptionsController::single(quint8 toggle) {
  return mpdSocket_->sendCommand(singleCmd + " " + QByteArray::number(toggle))
      .second;
}

bool PlaybackOptionsController::volume(int volChange) {
  return mpdSocket_
      ->sendCommand(volumeCmd + " " + QByteArray::number(volChange))
      .second;
}
