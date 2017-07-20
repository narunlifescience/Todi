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

#include "mpdsocket.h"

static const QByteArray okResponse("OK");
static const QByteArray okmpdResponse("OK MPD");
static const QByteArray oknResponse("OK\n");
static const QByteArray ackResponse("ACK");
static const QByteArray messageResponse("message");

const int MPDSocket::socketReadTimeOut_ = 5000;
const int MPDSocket::socketMaxReadAttempt_ = 9;

MPDSocket::MPDSocket(QObject *parent)
    : QTcpSocket(parent),
      hostname_(""),
      port_(0),
      passwd_(""),
      mpdCmdReply_(QByteArray(), false) {
  connect(this,
          static_cast<void (QTcpSocket::*)(const QAbstractSocket::SocketError)>(
              &QTcpSocket::error),
          this, &MPDSocket::onError);
}

void MPDSocket::connectToMPDHost(const QString &hostName, const quint16 port,
                                 const QString &password,
                                 const QIODevice::OpenMode mode) {
  if (!isConnected()) {
    QByteArray data;

    if (hostName != hostname_) hostname_ = hostName;
    if (port != port_) port_ = port;
    if (password != passwd_) passwd_ = password;

    if (hostname_.isEmpty() || port_ == 0) {
      qCritical() << "no valid hostname and/or port";
      qInfo() << hostname_ << port_;
      return;
    }

    connectToHost(hostname_, port_, mode);

    if (waitForConnected(socketReadTimeOut_)) {
      qInfo() << "MPD Connection established";
      data = readFromMPDSocket();

      if (data.isEmpty()) {
        qInfo() << "Couldn't connect";
        return;
      }

      if (data.startsWith(okmpdResponse)) {
        qInfo() << "MPD connected";
      }

      data.clear();

      if (!passwd_.isEmpty()) {
        qInfo() << "setting password...";
        write("password " + passwd_.toUtf8() + '\n');
        waitForBytesWritten(socketReadTimeOut_);
        if (!readFromMPDSocket().endsWith(oknResponse)) {
          qCritical() << "password rejected";
          close();
          return;
        }
      }
    }
  } else {
    qCritical() << "Couldn't connect - " << errorString() << error();
  }
}

void MPDSocket::disconnectFromMPDHost() {
  qInfo() << "Disconnect from MPD";
  if (isConnected()) {
    disconnectFromHost();
  }
  close();
}

QByteArray MPDSocket::readFromMPDSocket() {
  QByteArray data;
  int attempt = 0;
  while (isConnected()) {
    while (bytesAvailable() == 0 && isConnected()) {
      qDebug() << this << " Waiting for read data, attempt " << attempt;
      if (waitForReadyRead(socketReadTimeOut_)) {
        break;
      }
      qDebug() << "Wait for read failed - " << errorString();
      attempt++;
      if (attempt >= socketMaxReadAttempt_) {
        qCritical() << "ERROR: Timedout waiting for response";
        close();
        return QByteArray();
      }
    }

    data.append(readAll());

    if (data.endsWith(oknResponse) || data.startsWith(okResponse) ||
        data.startsWith(ackResponse)) {
      break;
    }
  }
  qDebug() << this << "Read:" << data << ", socket state: " << state();

  return data;
}

QPair<QByteArray, bool> MPDSocket::getMPDResponse() {
  mpdCmdReply_.first = readFromMPDSocket();
  mpdCmdReply_.second = mpdCmdReply_.first.endsWith(oknResponse);
  return mpdCmdReply_;
}

QPair<QByteArray, bool> MPDSocket::sendCommand(const QByteArray &command,
                                               const bool emitErrors,
                                               const bool retry) {
  qDebug() << "sending Command: " << command << emitErrors << retry;
  QPair<QByteArray, bool> mpdCmdResponse(QByteArray(), false);
  if (!isConnected()) {
    qCritical() << "Failed to send command to " << command
                << "- not connected!";
    return mpdCmdResponse;
  }

  if (write(command + '\n') == -1) {
    qCritical() << "Failed to write";
    // If we fail to write, dont wait for bytes to be written!!
    close();
  } else {
    const int timeout = 5000;
    qDebug() << "Timeout (ms): " << timeout;
    waitForBytesWritten(timeout);
    qDebug() << "Socket state after write: " << state();
    mpdCmdResponse = getMPDResponse();
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

void MPDSocket::onError(const QAbstractSocket::SocketError socketError) const {
  // Handle socket errors
  switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
      qCritical() << "The connection was refused by the peer (or timed out)";
      break;
    case QAbstractSocket::RemoteHostClosedError:
      qCritical() << "The remote host closed the connection";
      break;
    case QAbstractSocket::HostNotFoundError:
      qCritical() << "The host address was not found" << hostname_;
      break;
    case QAbstractSocket::SocketAccessError:
      qCritical() << "The socket operation failed because the application "
                     "lacked the required privileges";
      break;
    case QAbstractSocket::SocketResourceError:
      qCritical() << "The local system ran out of resources";
      break;
    case QAbstractSocket::SocketTimeoutError:
      qCritical() << "The socket operation timed out";
      break;
    case QAbstractSocket::DatagramTooLargeError:
      qCritical() << "The datagram was larger than the operating system's "
                     "limit";
      break;
    case QAbstractSocket::NetworkError:
      qCritical() << "An error occurred with the network";
      break;
    case QAbstractSocket::AddressInUseError:
      qCritical() << "The address specified " << hostname_
                  << " is already in use & was set to be exclusive.";
      break;
    case QAbstractSocket::SocketAddressNotAvailableError:
      qCritical() << "The address specified " << hostname_
                  << " does not belong to the host";
      break;
    case QAbstractSocket::UnsupportedSocketOperationError:
      qCritical() << "The requested socket operation is not supported by the "
                     "local operating system";
      break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
      qCritical() << "The socket is using a proxy & the proxy requires "
                     "authentication";
      break;
    case QAbstractSocket::SslHandshakeFailedError:
      qCritical() << "The SSL/TLS handshake failed, so the connection was "
                     "closed";
      break;
    case QAbstractSocket::UnfinishedSocketOperationError:
      qCritical() << "Used by QAbstractSocketEngine only, The last operation "
                     "attempted has not finished yet";
      break;
    case QAbstractSocket::ProxyConnectionRefusedError:
      qCritical() << "Could not contact the proxy server connection denied";
      break;
    case QAbstractSocket::ProxyConnectionClosedError:
      qCritical() << "The connection to the proxy server closed unexpectedly";
      break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
      qCritical() << "The connection to the proxy server timed out or the "
                     "proxy server stopped responding in the authentication "
                     "phase";
      break;
    case QAbstractSocket::ProxyNotFoundError:
      qCritical() << "The proxy address was not found";
      break;
    case QAbstractSocket::ProxyProtocolError:
      qCritical() << "The connection negotiation with the proxy server "
                     "failed, because the response from the proxy server "
                     "could not be understood";
      break;
    case QAbstractSocket::OperationError:
      qCritical() << "An operation was attempted while the socket was in a "
                     "state that did not permit it";
      break;
    case QAbstractSocket::SslInternalError:
      qCritical() << "The SSL library being used reported an internal error. "
                     "This is probably the result of a bad installation or "
                     "misconfiguration of the library";
      break;
    case QAbstractSocket::SslInvalidUserDataError:
      qCritical() << "Invalid data (certificate, key, cypher, etc.) was "
                     "provided and its use resulted in an error in the "
                     "SSL library";
      break;
    case QAbstractSocket::TemporaryError:
      qCritical() << "A temporary error occurred";
      break;
    case QAbstractSocket::UnknownSocketError:
      qCritical() << "An unidentified error occurred";
      break;
  }
}
