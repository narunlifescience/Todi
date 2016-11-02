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

void PlaybackOptionsController::consume(quint8 toggle) {
  mpdSocket_->sendCommand(consumeCmd + " " + QByteArray::number(toggle));
}

void PlaybackOptionsController::crossfade(int seconds) {
  mpdSocket_->sendCommand(crossfadeCmd + " " + QByteArray::number(seconds));
}

void PlaybackOptionsController::random(quint8 toggle) {
  mpdSocket_->sendCommand(randomCmd + " " + QByteArray::number(toggle));
}

void PlaybackOptionsController::repeat(quint8 toggle) {
  mpdSocket_->sendCommand(repeatCmd + " " + QByteArray::number(toggle));
}

void PlaybackOptionsController::setvol(quint8 vol) {
  mpdSocket_->sendCommand(setvolCmd + " " + QByteArray::number(vol));
}

void PlaybackOptionsController::single(quint8 toggle) {
  mpdSocket_->sendCommand(singleCmd + " " + QByteArray::number(toggle));
}

void PlaybackOptionsController::volume(int volChange) {
  mpdSocket_->sendCommand(volumeCmd + " " + QByteArray::number(volChange));
}
