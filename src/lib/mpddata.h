#ifndef STATUS_H
#define STATUS_H

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

#include <QObject>
#include <memory>

#include "mpdmodel.h"

class MPDSocket;

class MPDdata : public QObject {
  Q_OBJECT
 public:
  explicit MPDdata(QObject *parent = nullptr,
                   std::shared_ptr<MPDSocket> mpdSocket = nullptr);
  ~MPDdata();
  void getMPDStatus();
  void getMPDStats();
  void getMPDSongMetadata();
  void getMPDPlaylistInfo();

  // MPD status
  qint8 volume() const;
  bool consume() const;
  bool repeat() const;
  bool single() const;
  bool random() const;
  quint32 playlist() const;
  quint32 playlistLength() const;
  qint32 crossFade() const;
  MPDPlaybackState state() const;
  qint32 song() const;
  qint32 songId() const;
  qint32 nextSong() const;
  qint32 nextSongId() const;
  qint32 timeElapsed() const;
  qint32 timeTotal() const;
  quint16 bitrate() const;
  quint16 samplerate() const;
  quint8 bits() const;
  quint8 channels() const;
  qint32 updatingDb() const;
  const QString &error() const;
  MPDStatusValues *getStatusValues() const;

  // MPD stats
  quint32 artists() const;
  quint32 albums() const;
  quint32 songs() const;
  quint32 uptime() const;
  quint32 playtime() const;
  quint32 dbPlaytime() const;
  time_t dbUpdate() const;
  MPDStatsValues *getStatsValues() const;

  // MPD song metadata
  QString file() const;
  QString artist() const;
  QString album() const;
  QString albumId() const;
  QString albumArtist() const;
  QString title() const;
  quint16 track() const;
  QString name() const;
  QString genre() const;
  quint16 date() const;
  QString composer() const;
  QString performer() const;
  QString comment() const;
  quint8 disc() const;
  quint16 time() const;
  qint32 id() const;
  QString lastModified() const;
  uint pos() const;
  MPDSongMetadata *getSongMetadataValues() const;

  QList<MPDSongMetadata *> *getPlaylistinfoValues() const;

 signals:
  void MPDStatusUpdated();
  void MPDStatsUpdated();
  void MPDSongMetadataUpdated(QString filename);
  void MPDPlaylistinfoUpdated(QList<MPDSongMetadata *> *playlistQueue);

 private:
  std::shared_ptr<MPDSocket> mpdSocket_;
  MPDStatusValues *statusValues_;
  MPDStatsValues *statsValues_;
  MPDSongMetadata *songMetadataValues_;
  QList<MPDSongMetadata *> *playlistQueue_;
  static const QByteArray statusCommand;
  static const QByteArray statsCommand;
  static const QByteArray songMetadataCommand;
  const static QByteArray playlistinfoCommand;
};

#endif  // STATUS_H
