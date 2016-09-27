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

#include "mpdclient.h"
#include "mpdsocket.h"
#include "commandcontroller.h"
#include "mpdmodel.h"

MPDClient::MPDClient(QObject *parent) : QObject (parent),
  mpdSocket_(new MPDSocket(this)),
  commandCtrlr_(new CommandController(this, mpdSocket_)) {
}

MPDClient::~MPDClient(){}

void MPDClient::connectToHost(const QString &hostName, const quint16 port,
                              const QString &password) {
  mpdSocket_->connectToMPDHost(hostName, port, password);
  commandCtrlr_->sendCommand("pause", true, true);
  qRegisterMetaType<MPDStatusValues>("MPDStatusValues");
}

void MPDClient::disconnectFromHost() const {
  mpdSocket_->disconnectFromMPDHost();
}
