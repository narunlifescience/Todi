#ifndef STATUS_H
#define STATUS_H

#include <QObject>
#include <memory>

#include "mpdmodel.h"

class CommandController;

class MPDdata : public QObject {
  Q_OBJECT
 public:
  explicit MPDdata(QObject *parent = nullptr,
          std::shared_ptr<CommandController> cmdctrlr = nullptr);
  ~MPDdata();
  void updateMpdStatus(const MPDStatusValues &newStatusValues);
  void updateMpdStats(const MPDStatsValues &newStatsValues);
  void getMPDStatus();
  void getMPDStats();

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

signals:
  void MPDStatusUpdated();
  void MPDStatsUpdated();

 private:
  std::shared_ptr<CommandController> cmdCtrlr_;
  MPDStatusValues statusValues_;
  MPDStatsValues statsValues_;
  static const QByteArray statusCommand;
  static const QByteArray statsCommand;
};

#endif  // STATUS_H
