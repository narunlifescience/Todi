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

#ifndef MPDSOCKET_H
#define MPDSOCKET_H

#include <QTcpSocket>

class MPDSocket : public QTcpSocket {
  Q_OBJECT
 public:
  explicit MPDSocket(QObject *parent = nullptr);
  void connectToMPDHost(const QString &hostName, const quint16 port,
                        const QString &password,
                        const QIODevice::OpenMode mode = QIODevice::ReadWrite);
  void disconnectFromMPDHost();
  QByteArray readFromMPDSocket();
  QPair<QByteArray, bool> getMPDResponse();
  inline bool isConnected() const {
    return (state() == QAbstractSocket::ConnectedState);
  }

 public slots:
  void onError(const QAbstractSocket::SocketError socketError) const;

 private:
  QString hostname_;
  quint16 port_;
  QString passwd_;
  QPair<QByteArray, bool> mpdCmdReply_;
  static const int socketReadTimeOut_;
  static const int socketMaxReadAttempt_;
};

#endif  // MPDSOCKET_H
