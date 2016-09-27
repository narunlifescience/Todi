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

#include "commandcontroller.h"
#include "mpdsocket.h"

CommandController::CommandController(QObject *parent,
                                     std::shared_ptr<MPDSocket> socket)
    : QObject(parent), mpdSocket_(socket) {}

QPair<QByteArray, bool> CommandController::sendCommand(
    const QByteArray &command, const bool emitErrors, const bool retry) {
  qDebug() << "sending Command: " << command << emitErrors << retry;
  QPair<QByteArray, bool> mpdCmdResponse(QByteArray(), false);
  if (!mpdSocket_->isConnected()) {
    qCritical() << "Failed to send command to " << command
                << "- not connected!";
    return mpdCmdResponse;
  }

  if (mpdSocket_->write(command + '\n') == -1) {
    qCritical() << "Failed to write";
    // If we fail to write, dont wait for bytes to be written!!
    mpdSocket_->close();
  } else {
    const int timeout = 5000;
    qDebug() << "Timeout (ms): " << timeout;
    mpdSocket_->waitForBytesWritten(timeout);
    qDebug() << "Socket state after write: " << mpdSocket_->state();
    mpdCmdResponse = mpdSocket_->getMPDResponse();
  }

  if (!mpdCmdResponse.second) {
    qWarning() << "sent Command: " << command << " failed!";

    // Try one more time if retry status is true
    if (mpdCmdResponse.first.isEmpty() && retry) {
      return sendCommand(command, emitErrors, false);
    }
    return mpdCmdResponse;
  }

  qDebug() << "sentCommand: " << command << " sucessful!";
  return mpdCmdResponse;
}
