#ifndef STATUS_H
#define STATUS_H

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
  void updateMpdStatus(const MPDStatusValues &newStatusValues);
  void updateMpdStats(const MPDStatsValues &newStatsValues);
  void updateMpdSongMetadata(const MPDSongMetadata &newSongMetadataValues);
  void getMPDStatus();
  void getMPDStats();
  void getMPDSongMetadata();

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
  MPDStatusValues getStatusValues() const;

  // MPD stats
  quint32 artists() const;
  quint32 albums() const;
  quint32 songs() const;
  quint32 uptime() const;
  quint32 playtime() const;
  quint32 dbPlaytime() const;
  time_t dbUpdate() const;
  MPDStatsValues getStatsValues() const;

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
  MPDSongMetadata getSongMetadataValues() const;

signals:
  void MPDStatusUpdated();
  void MPDStatsUpdated();
  void MPDSongMetadataUpdated(QString filename);

 private:
  std::shared_ptr<MPDSocket> mpdSocket_;
  MPDStatusValues statusValues_;
  MPDStatsValues statsValues_;
  MPDSongMetadata songMetadataValues_;
  static const QByteArray statusCommand;
  static const QByteArray statsCommand;
  static const QByteArray songMetadataCommand;
};

#endif  // STATUS_H
