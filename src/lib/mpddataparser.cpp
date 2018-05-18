#include "mpddataparser.h"
#include "mpdfilemodel.h"

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

// MPD song metadata look up keys
static const QByteArray songMetadataFileKey("file: ");
static const QByteArray songMetadataTimeKey("Time: ");
static const QByteArray songMetadataAlbumKey("Album: ");
static const QByteArray songMetadataArtistKey("Artist: ");
static const QByteArray songMetadataAlbumArtistKey("AlbumArtist: ");
static const QByteArray songMetadataComposerKey("Composer: ");
static const QByteArray songMetadataTitleKey("Title: ");
static const QByteArray songMetadataTrackKey("Track: ");
static const QByteArray songMetadataIdKey("Id: ");
static const QByteArray songMetadataDiscKey("Disc: ");
static const QByteArray songMetadataDateKey("Date: ");
static const QByteArray songMetadataGenreKey("Genre: ");
static const QByteArray songMetadataNameKey("Name: ");
static const QByteArray songMetadataAlbumIdKey("MUSICBRAINZ_ALBUMID: ");
static const QByteArray songMetadataPerformerKey("Performer: ");
static const QByteArray songMetadataCommentKey("Comment: ");
static const QByteArray songMetadataLastModifiedKey("Last-Modified: ");
static const QByteArray songMetadataPosKey("Pos: ");

// MPD look up values
static const QByteArray okValue("OK");
static const QByteArray enabledValue("1");
static const QByteArray PlayValue("play");
static const QByteArray StopValue("stop");

void MPDdataParser::parseStatus(const QByteArray &data,
                                MPDStatusValues *statusValues) {
  QList<QByteArray> lines = data.split('\n');

  foreach (const QByteArray &line, lines) {
    if (line.startsWith(statusVolumeKey)) {
      statusValues->volume =
          static_cast<qint8>(line.mid(statusVolumeKey.length()).toInt());
    } else if (line.startsWith(statusConsumeKey)) {
      statusValues->consume =
          (line.mid(statusConsumeKey.length()) == enabledValue) ? true : false;
    } else if (line.startsWith(statusRepeatKey)) {
      statusValues->repeat =
          (line.mid(statusRepeatKey.length()) == enabledValue) ? true : false;
    } else if (line.startsWith(statusSingleKey)) {
      statusValues->single =
          (line.mid(statusSingleKey.length()) == enabledValue) ? true : false;
    } else if (line.startsWith(statusRandomKey)) {
      statusValues->random =
          (line.mid(statusRandomKey.length()) == enabledValue) ? true : false;
    } else if (line.startsWith(statusPlaylistKey)) {
      statusValues->playlist = line.mid(statusPlaylistKey.length()).toUInt();
    } else if (line.startsWith(statusPlaylistLengthKey)) {
      statusValues->playlistLength = static_cast<quint32>(
          line.mid(statusPlaylistLengthKey.length()).toInt());
    } else if (line.startsWith(statusCrossfadeKey)) {
      statusValues->crossFade = line.mid(statusCrossfadeKey.length()).toInt();
    } else if (line.startsWith(statusStateKey)) {
      QByteArray value = line.mid(statusStateKey.length());
      if (PlayValue == value) {
        statusValues->state = MPDPlaybackState::Playing;
      } else if (StopValue == value) {
        statusValues->state = MPDPlaybackState::Stopped;
      } else {
        statusValues->state = MPDPlaybackState::Paused;
      }
    } else if (line.startsWith(statusSongKey)) {
      statusValues->song = line.mid(statusSongKey.length()).toInt();
    } else if (line.startsWith(statusSongIdKey)) {
      statusValues->songId = line.mid(statusSongIdKey.length()).toInt();
    } else if (line.startsWith(statusNextSongKey)) {
      statusValues->nextSong = line.mid(statusNextSongKey.length()).toInt();
    } else if (line.startsWith(statusNextSongIdKey)) {
      statusValues->nextSongId = line.mid(statusNextSongIdKey.length()).toInt();
    } else if (line.startsWith(statusTimeKey)) {
      QList<QByteArray> values = line.mid(statusTimeKey.length()).split(':');
      if (values.length() > 1) {
        statusValues->timeElapsed = values.at(0).toInt();
        statusValues->timeTotal = values.at(1).toInt();
      }
      values.clear();
    } else if (line.startsWith(statusBitrateKey)) {
      statusValues->bitrate =
          static_cast<quint16>(line.mid(statusBitrateKey.length()).toUInt());
    } else if (line.startsWith(statusAudioKey)) {
      QList<QByteArray> values = line.mid(statusAudioKey.length()).split(':');
      if (values.length() == 3) {
        statusValues->samplerate = static_cast<quint16>(values.at(0).toUInt());
        statusValues->bits = static_cast<quint8>(values.at(1).toUInt());
        statusValues->channels = static_cast<quint8>(values.at(2).toUInt());
      }
      values.clear();
    } else if (line.startsWith(statusUpdatingDbKey)) {
      statusValues->updatingDb = line.mid(statusUpdatingDbKey.length()).toInt();
    } else if (line.startsWith(statusErrorKey)) {
      statusValues->error =
          QString::fromUtf8(line.mid(statusErrorKey.length()));
    }
  }
  lines.clear();
}

void MPDdataParser::parseStats(const QByteArray &data,
                               MPDStatsValues *statsValues) {
  QList<QByteArray> lines = data.split('\n');

  foreach (const QByteArray &line, lines) {
    if (line.startsWith(statsArtistsKey)) {
      statsValues->artists = line.mid(statsArtistsKey.length()).toUInt();
    } else if (line.startsWith(statsAlbumsKey)) {
      statsValues->albums = line.mid(statsAlbumsKey.length()).toUInt();
    } else if (line.startsWith(statsSongsKey)) {
      statsValues->songs = line.mid(statsSongsKey.length()).toUInt();
    } else if (line.startsWith(statsUptimeKey)) {
      statsValues->uptime = line.mid(statsUptimeKey.length()).toUInt();
    } else if (line.startsWith(statsPlaytimeKey)) {
      statsValues->playtime = line.mid(statsPlaytimeKey.length()).toUInt();
    } else if (line.startsWith(statsDbPlaytimeKey)) {
      statsValues->dbPlaytime = line.mid(statsDbPlaytimeKey.length()).toUInt();
    } else if (line.startsWith(statsDbUpdateKey)) {
      statsValues->dbUpdate = line.mid(statsDbUpdateKey.length()).toUInt();
    }
  }
  lines.clear();
}

void MPDdataParser::parseSongMetadata(const QList<QByteArray> &data,
                                      MPDSongMetadata *songMetadataValues) {
  foreach (const QByteArray &line, data) {
    if (line.startsWith(songMetadataFileKey)) {
      songMetadataValues->file =
          QString::fromUtf8(line.mid(songMetadataFileKey.length()));
    } else if (line.startsWith(songMetadataTimeKey)) {
      songMetadataValues->time = line.mid(songMetadataTimeKey.length()).toInt();
    } else if (line.startsWith(songMetadataAlbumKey)) {
      songMetadataValues->album =
          QString::fromUtf8(line.mid(songMetadataAlbumKey.length()));
    } else if (line.startsWith(songMetadataArtistKey)) {
      songMetadataValues->artist =
          QString::fromUtf8(line.mid(songMetadataArtistKey.length()));
    } else if (line.startsWith(songMetadataAlbumArtistKey)) {
      songMetadataValues->albumArtist =
          QString::fromUtf8(line.mid(songMetadataAlbumArtistKey.length()));
    } else if (line.startsWith(songMetadataComposerKey)) {
      songMetadataValues->comment =
          QString::fromUtf8(line.mid(songMetadataComposerKey.length()));
    } else if (line.startsWith(songMetadataTitleKey)) {
      songMetadataValues->title =
          QString::fromUtf8(line.mid(songMetadataTitleKey.length()));
    } else if (line.startsWith(songMetadataTrackKey)) {
      int v = line.mid(songMetadataTrackKey.length()).split('/').at(0).toInt();
      songMetadataValues->track = v < 0 ? 0 : v;
    } else if (line.startsWith(songMetadataIdKey)) {
      songMetadataValues->id = line.mid(songMetadataIdKey.length()).toUInt();
    } else if (line.startsWith(songMetadataDiscKey)) {
      int v = line.mid(songMetadataDiscKey.length()).split('/').at(0).toInt();
      songMetadataValues->disc = v < 0 ? 0 : v;
    } else if (line.startsWith(songMetadataDateKey)) {
      QByteArray value = line.mid(songMetadataDateKey.length());
      int v = value.length() > 4 ? value.left(4).toUInt() : value.toUInt();
      songMetadataValues->date = v < 0 ? 0 : v;
    } else if (line.startsWith(songMetadataGenreKey)) {
      songMetadataValues->genre =
          QString::fromUtf8(line.mid(songMetadataGenreKey.length()));
    } else if (line.startsWith(songMetadataNameKey)) {
      songMetadataValues->name =
          QString::fromUtf8(line.mid(songMetadataNameKey.length()));
    } else if (line.startsWith(songMetadataAlbumIdKey)) {
      songMetadataValues->albumId =
          QString::fromUtf8(line.mid(songMetadataAlbumIdKey.length()));
    } else if (line.startsWith(songMetadataPerformerKey)) {
      songMetadataValues->performer =
          QString::fromUtf8(line.mid(songMetadataPerformerKey.length()));
    } else if (line.startsWith(songMetadataCommentKey)) {
      songMetadataValues->comment =
          QString::fromUtf8(line.mid(songMetadataCommentKey.length()));
    } else if (line.startsWith(songMetadataLastModifiedKey)) {
      songMetadataValues->lastModified =
          QString::fromUtf8(line.mid(songMetadataLastModifiedKey.length()));
    } else if (line.startsWith(songMetadataPosKey)) {
      songMetadataValues->pos =
          QString::fromUtf8(line.mid(songMetadataPosKey.length())).toUInt();
    }
  }
}

void MPDdataParser::parsePlaylistQueue(
    const QByteArray &data, QList<MPDSongMetadata *> *playlistQueue) {
  QList<QByteArray> songblock;
  QList<QByteArray> lines = data.split('\n');
  foreach (const QByteArray &line, lines) {
    if (line == okValue) continue;

    if (line.startsWith(songMetadataFileKey)) {
      MPDSongMetadata *songmetadata = new MPDSongMetadata();
      parseSongMetadata(songblock, songmetadata);
      playlistQueue->append(songmetadata);
      songblock.clear();
    }

    if (!line.isEmpty()) songblock.append(line);
  }
}

void MPDdataParser::parseFolderView(const QByteArray &data,
                                    RootItem *rootitem) {
  QList<QByteArray> lines = data.split('\n');

  Item *currentDir = rootitem;
  QStringList currentDirList;

  int amountOfLines = lines.size();
  for (int i = 0; i < amountOfLines; i++) {
    QString line(lines.at(i));

    if (line.startsWith("file: ")) {
      line.remove(0, 6);
      QStringList parts = line.split("/");

      if (currentDir->type() == Item::Type::TypeRoot)
        static_cast<RootItem *>(currentDir)
            ->insertFile(parts.at(parts.size() - 1));
      else
        static_cast<FolderItem *>(currentDir)
            ->insertFile(parts.at(parts.size() - 1));
    } else if (line.startsWith("directory: ")) {
      line.remove(0, 11);
      QStringList parts = line.split("/");

      /* Check how much matches */
      int depth = 0;
      for (int j = 0; j < currentDirList.size() && j < parts.size(); j++) {
        if (currentDirList.at(j) != parts.at(j)) break;
        depth++;
      }

      for (int j = currentDirList.size(); j > depth; j--) {
        currentDir = currentDir->parent();
      }

      if (currentDir->type() == Item::Type::TypeRoot)
        currentDir = static_cast<RootItem *>(currentDir)
                         ->createDirectory(parts.at(parts.size() - 1));
      else
        currentDir = static_cast<FolderItem *>(currentDir)
                         ->createDirectory(parts.at(parts.size() - 1));

      currentDirList = parts;
    }
  }
}
