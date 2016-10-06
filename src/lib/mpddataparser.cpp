#include "mpddataparser.h"

// MPD status look up keys
static const QByteArray statusVolumeKey("volume: ");
static const QByteArray statusConsumeKey("consume: ");
static const QByteArray statusRepeatKey("repeat: ");
static const QByteArray statusSingleKey("single: ");
static const QByteArray statusRandomKey("random: ");
static const QByteArray statusPlaylistKey("playlist: ");
static const QByteArray statusPlaylistLengthKey("playlistlength: ");
static const QByteArray statusCrossfadeKey("xfade: ");
static const QByteArray statusStateKey("state: ");
static const QByteArray statusSongKey("song: ");
static const QByteArray statusSongIdKey("songid: ");
static const QByteArray statusNextSongKey("nextsong: ");
static const QByteArray statusNextSongIdKey("nextsongid: ");
static const QByteArray statusTimeKey("time: ");
static const QByteArray statusBitrateKey("bitrate: ");
static const QByteArray statusAudioKey("audio: ");
static const QByteArray statusUpdatingDbKey("updating_db: ");
static const QByteArray statusErrorKey("error: ");

// MPD stats look up keys
static const QByteArray statsArtistsKey("artists: ");
static const QByteArray statsAlbumsKey("albums: ");
static const QByteArray statsSongsKey("songs: ");
static const QByteArray statsUptimeKey("uptime: ");
static const QByteArray statsPlaytimeKey("playtime: ");
static const QByteArray statsDbPlaytimeKey("db_playtime: ");
static const QByteArray statsDbUpdateKey("db_update: ");

// MPD look up values
static const QByteArray enabledValue("1");
static const QByteArray PlayValue("play");
static const QByteArray StopValue("stop");

MPDStatusValues MPDdataParser::parseStatus(const QByteArray &data) {
  MPDStatusValues statusValues;
  QList<QByteArray> lines = data.split('\n');

  foreach (const QByteArray &line, lines) {
    if (line.startsWith(statusVolumeKey)) {
      statusValues.volume =
          static_cast<qint8>(line.mid(statusVolumeKey.length()).toInt());
    } else if (line.startsWith(statusConsumeKey)) {
      statusValues.consume =
          (line.mid(statusConsumeKey.length()) == enabledValue) ? true : false;
    } else if (line.startsWith(statusRepeatKey)) {
      statusValues.repeat =
          (line.mid(statusRepeatKey.length()) == enabledValue) ? true : false;
    } else if (line.startsWith(statusSingleKey)) {
      statusValues.single =
          (line.mid(statusSingleKey.length()) == enabledValue) ? true : false;
    } else if (line.startsWith(statusRandomKey)) {
      statusValues.random =
          (line.mid(statusRandomKey.length()) == enabledValue) ? true : false;
    } else if (line.startsWith(statusPlaylistKey)) {
      statusValues.playlist = line.mid(statusPlaylistKey.length()).toUInt();
    } else if (line.startsWith(statusPlaylistLengthKey)) {
      statusValues.playlistLength = static_cast<quint32>(
          line.mid(statusPlaylistLengthKey.length()).toInt());
    } else if (line.startsWith(statusCrossfadeKey)) {
      statusValues.crossFade = line.mid(statusCrossfadeKey.length()).toInt();
    } else if (line.startsWith(statusStateKey)) {
      QByteArray value = line.mid(statusStateKey.length());
      if (PlayValue == value) {
        statusValues.state = MPDPlaybackState::Playing;
      } else if (StopValue == value) {
        statusValues.state = MPDPlaybackState::Stopped;
      } else {
        statusValues.state = MPDPlaybackState::Paused;
      }
    } else if (line.startsWith(statusSongKey)) {
      statusValues.song = line.mid(statusSongKey.length()).toInt();
    } else if (line.startsWith(statusSongIdKey)) {
      statusValues.songId = line.mid(statusSongIdKey.length()).toInt();
    } else if (line.startsWith(statusNextSongKey)) {
      statusValues.nextSong = line.mid(statusNextSongKey.length()).toInt();
    } else if (line.startsWith(statusNextSongIdKey)) {
      statusValues.nextSongId = line.mid(statusNextSongIdKey.length()).toInt();
    } else if (line.startsWith(statusTimeKey)) {
      QList<QByteArray> values = line.mid(statusTimeKey.length()).split(':');
      if (values.length() > 1) {
        statusValues.timeElapsed = values.at(0).toInt();
        statusValues.timeTotal = values.at(1).toInt();
      }
    } else if (line.startsWith(statusBitrateKey)) {
      statusValues.bitrate =
          static_cast<quint16>(line.mid(statusBitrateKey.length()).toUInt());
    } else if (line.startsWith(statusAudioKey)) {
      QList<QByteArray> values = line.mid(statusAudioKey.length()).split(':');
      if (values.length() == 3) {
        statusValues.samplerate = static_cast<quint16>(values.at(0).toUInt());
        statusValues.bits = static_cast<quint8>(values.at(1).toUInt());
        statusValues.channels = static_cast<quint8>(values.at(2).toUInt());
      }
    } else if (line.startsWith(statusUpdatingDbKey)) {
      statusValues.updatingDb = line.mid(statusUpdatingDbKey.length()).toInt();
    } else if (line.startsWith(statusErrorKey)) {
      statusValues.error = QString::fromUtf8(line.mid(statusErrorKey.length()));
    }
  }
  return statusValues;
}

MPDStatsValues MPDdataParser::parseStats(const QByteArray &data) {
  MPDStatsValues statsValues;
  QList<QByteArray> lines = data.split('\n');

  foreach (const QByteArray &line, lines) {
    if (line.startsWith(statsArtistsKey)) {
      statsValues.artists = line.mid(statsArtistsKey.length()).toUInt();
    } else if (line.startsWith(statsAlbumsKey)) {
      statsValues.albums = line.mid(statsAlbumsKey.length()).toUInt();
    } else if (line.startsWith(statsSongsKey)) {
      statsValues.songs = line.mid(statsSongsKey.length()).toUInt();
    } else if (line.startsWith(statsUptimeKey)) {
      statsValues.uptime = line.mid(statsUptimeKey.length()).toUInt();
    } else if (line.startsWith(statsPlaytimeKey)) {
      statsValues.playtime = line.mid(statsPlaytimeKey.length()).toUInt();
    } else if (line.startsWith(statsDbPlaytimeKey)) {
      statsValues.dbPlaytime = line.mid(statsDbPlaytimeKey.length()).toUInt();
    } else if (line.startsWith(statsDbUpdateKey)) {
      statsValues.dbUpdate = line.mid(statsDbUpdateKey.length()).toUInt();
    }
  }
  return statsValues;
}
