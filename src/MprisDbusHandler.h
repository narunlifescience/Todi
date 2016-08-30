#ifndef MPRISDBUSHANDLER_H
#define MPRISDBUSHANDLER_H

#include "MprisHandler.h"

#include <QString>
#include <QDBusInterface>

class MprisDBusHandler : public MprisHandler {
 public:
  MprisDBusHandler(const QString& service, const QString& path,
                   const QString& interface);

  void MediaPlaybackNoArgs(PlayBack playback) override;
  MprisHandler::PlayBackStatus MediaPlaybackStatus() override;
  void getPlayingAlbumCover() override;

 private:
  QString service_;
  QString path_;
  QString interface_;
};

#endif  // MPRISDBUSHANDLER_H
