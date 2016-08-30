#include "MprisHandler.h"
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDebug>
#include <QtGlobal>
#include "MprisDbusHandler.h"

// Services
const char* MprisHandler::ClementineMpris2Service =
    "org.mpris.MediaPlayer2.clementine";

namespace {
// Clementine Player
const char* ClementineMpris2Path = "/org/mpris/MediaPlayer2";
const char* ClementineMpris2Interface = "org.mpris.MediaPlayer2.Player";
}

MprisHandler* MprisHandler::mprisPlayerHandle_ = 0;

MprisHandler* MprisHandler::GetMprisPlayerHandle() {
  if (!mprisPlayerHandle_) {
    // For gnome
    if (QDBusConnection::sessionBus().interface()->isServiceRegistered(
            ClementineMpris2Service)) {
      mprisPlayerHandle_ =
          new MprisDBusHandler(ClementineMpris2Service, ClementineMpris2Path,
                               ClementineMpris2Interface);
    } else {
      qDebug() << "no supported Mpris service available";
    }
  }
  return mprisPlayerHandle_;
}
