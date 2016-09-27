#ifndef MPDMODEL_H
#define MPDMODEL_H

#include <QObject>

enum class MPDPlaybackState {
  Inactive,
  Paused,
  Playing,
  Stopped,
};

struct MPDStatusValues {
  MPDStatusValues()
      : volume(0),
        consume(false),
        repeat(false),
        single(false),
        random(false),
        playlist(0),
        playlistLength(0),
        crossFade(0),
        state(MPDPlaybackState::Inactive),
        song(-1),
        songId(-1),
        nextSong(-1),
        nextSongId(-1),
        timeElapsed(-1),
        timeTotal(-1),
        bitrate(0),
        samplerate(0),
        bits(0),
        channels(0),
        updatingDb(-1) {}
  qint8 volume;
  bool consume;
  bool repeat;
  bool single;
  bool random;
  quint32 playlist;
  quint32 playlistLength;
  qint32 crossFade;
  MPDPlaybackState state;
  qint32 song;
  qint32 songId;
  qint32 nextSong;
  qint32 nextSongId;
  qint32 timeElapsed;
  qint32 timeTotal;
  quint16 bitrate;
  quint16 samplerate;
  quint8 bits;
  quint8 channels;
  qint32 updatingDb;
  QString error;
};

struct MPDStatsValues {
  MPDStatsValues()
      : artists(0),
        albums(0),
        songs(0),
        uptime(0),
        playtime(0),
        dbPlaytime(0) {}
  quint32 artists;
  quint32 albums;
  quint32 songs;
  quint32 uptime;
  quint32 playtime;
  quint32 dbPlaytime;
  time_t dbUpdate;
};

#endif  // MPDMODEL_H
