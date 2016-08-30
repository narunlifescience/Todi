#include "MprisDbusHandler.h"

#include <QCoreApplication>
#include <QDBusInterface>
#include <QDBusReply>

#include <stdint.h>
#include <QDebug>

MprisDBusHandler::MprisDBusHandler(const QString& service, const QString& path,
                                   const QString& interface)
    : service_(service), path_(path), interface_(interface) {
}

void MprisDBusHandler::MediaPlaybackNoArgs(PlayBack playback) {
  QDBusInterface iface(service_, path_, interface_,
                       QDBusConnection::sessionBus());
  if (service_ == MprisHandler::ClementineMpris2Service) {
    switch (playback) {
      case MprisHandler::PlayBack::Play:
        iface.call("Play");
        break;
      case MprisHandler::PlayBack::Pause:
        iface.call("Pause");
        break;
      case MprisHandler::PlayBack::PlayPause:
        iface.call("PlayPause");
        break;
      case MprisHandler::PlayBack::Next:
        iface.call("Next");
        break;
      case MprisHandler::PlayBack::Previous:
        iface.call("Previous");
        break;
      case MprisHandler::PlayBack::Stop:
        iface.call("Stop");
        break;
    }

  } else {
    qDebug() << "unknown service : " << MprisHandler::ClementineMpris2Service;
  }
}

MprisHandler::PlayBackStatus MprisDBusHandler::MediaPlaybackStatus() {
  QDBusInterface iface(service_, path_, interface_,
                       QDBusConnection::sessionBus());
  if (service_ == MprisHandler::ClementineMpris2Service) {
    QString status = qvariant_cast<QString>(iface.property("PlaybackStatus"));
    if (status == "Paused") {
      return MprisHandler::PlayBackStatus::Paused;
    } else if (status == "Playing") {
      return MprisHandler::PlayBackStatus::Playing;
    } else if (status == "Stopped") {
      return MprisHandler::PlayBackStatus::Stopped;
    }
    return MprisHandler::PlayBackStatus::InvalidPlaybackStatus;
  }
  return MprisHandler::PlayBackStatus::InvalidPlaybackStatus;
}

void MprisDBusHandler::getPlayingAlbumCover() {
  QDBusInterface iface(service_, path_, interface_,
                       QDBusConnection::sessionBus());
}
