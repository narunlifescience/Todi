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

#ifndef MPDCLIENT_H
#define MPDCLIENT_H

#include <QObject>
#include <memory>

class MPDSocket;
class CommandController;
class MPDdata;
class PlaybackController;

class MPDClient : public QObject {
  Q_OBJECT

 public:
  MPDClient(QObject *parent = nullptr);
  ~MPDClient();

  bool connectToHost(const QString &hostName, const quint16 port,
                     const QString &password);
  void disconnectFromHost() const;

  std::shared_ptr<MPDdata> getSharedMPDdataPtr() const;
  std::shared_ptr<PlaybackController> getSharedPlaybackControllerPtr() const;

 private:
  std::shared_ptr<MPDSocket> mpdSocket_;

 public:
  std::shared_ptr<MPDdata> dataAccess_;
  std::shared_ptr<PlaybackController> playbackCtrlr_;
};

#endif  // MPDCLIENT_H
