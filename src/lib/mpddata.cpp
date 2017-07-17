#include "mpddata.h"
#include "mpddataparser.h"
#include "mpdsocket.h"

#include <QDebug>

const QByteArray MPDdata::statusCommand = "status";
const QByteArray MPDdata::statsCommand = "stats";
const QByteArray MPDdata::songMetadataCommand = "currentsong";

MPDdata::MPDdata(QObject *parent, std::shared_ptr<MPDSocket> mpdSocket)
    : QObject(parent), mpdSocket_(mpdSocket) {}

MPDdata::~MPDdata() {}

void MPDdata::updateMpdStatus(const MPDStatusValues &newStatusValues) {
  statusValues_ = newStatusValues;
}

void MPDdata::updateMpdStats(const MPDStatsValues &newStatsValues) {
  statsValues_ = newStatsValues;
}

void MPDdata::updateMpdSongMetadata(
    const MPDSongMetadata &newSongMetadataValues) {
  songMetadataValues_ = newSongMetadataValues;
}

void MPDdata::getMPDStatus() {
  QPair<QByteArray, bool> mpdStatus(mpdSocket_->sendCommand(statusCommand));
  if (mpdStatus.second) {
    statusValues_ = MPDdataParser::parseStatus(mpdStatus.first);
    emit MPDStatusUpdated();
  }
}

void MPDdata::getMPDStats() {
  QPair<QByteArray, bool> mpdStats(mpdSocket_->sendCommand(statsCommand));
  if (mpdStats.second) {
    statsValues_ = MPDdataParser::parseStats(mpdStats.first);
    emit MPDStatsUpdated();
  }
}

void MPDdata::getMPDSongMetadata() {
  QPair<QByteArray, bool> mpdSongMetadata(
      mpdSocket_->sendCommand(songMetadataCommand));
  if (mpdSongMetadata.second) {
    songMetadataValues_ =
        MPDdataParser::parseSongMetadata(mpdSongMetadata.first);
    emit MPDSongMetadataUpdated(songMetadataValues_.file);
  }
}

// MPD status
qint8 MPDdata::volume() const { return statusValues_.volume; }

bool MPDdata::consume() const { return statusValues_.consume; }

bool MPDdata::repeat() const { return statusValues_.repeat; }

bool MPDdata::single() const { return statusValues_.single; }

bool MPDdata::random() const { return statusValues_.random; }

quint32 MPDdata::playlist() const { return statusValues_.playlist; }

quint32 MPDdata::playlistLength() const { return statusValues_.playlistLength; }

qint32 MPDdata::crossFade() const { return statusValues_.crossFade; }

MPDPlaybackState MPDdata::state() const { return statusValues_.state; }

qint32 MPDdata::song() const { return statusValues_.song; }

qint32 MPDdata::songId() const { return statusValues_.songId; }

qint32 MPDdata::nextSong() const { return statusValues_.nextSong; }

qint32 MPDdata::nextSongId() const { return statusValues_.nextSongId; }

qint32 MPDdata::timeElapsed() const { return statusValues_.timeElapsed; }

qint32 MPDdata::timeTotal() const { return statusValues_.timeTotal; }

quint16 MPDdata::bitrate() const { return statusValues_.bitrate; }

quint16 MPDdata::samplerate() const { return statusValues_.samplerate; }

quint8 MPDdata::bits() const { return statusValues_.bits; }

quint8 MPDdata::channels() const { return statusValues_.channels; }

qint32 MPDdata::updatingDb() const { return statusValues_.updatingDb; }

const QString &MPDdata::error() const { return statusValues_.error; }

MPDStatusValues MPDdata::getStatusValues() const { return statusValues_; }

// MPD stats
quint32 MPDdata::artists() const { return statsValues_.artists; }

quint32 MPDdata::albums() const { return statsValues_.albums; }

quint32 MPDdata::songs() const { return statsValues_.songs; }

quint32 MPDdata::uptime() const { return statsValues_.uptime; }

quint32 MPDdata::playtime() const { return statsValues_.playtime; }

quint32 MPDdata::dbPlaytime() const { return statsValues_.dbPlaytime; }

time_t MPDdata::dbUpdate() const { return statsValues_.dbUpdate; }

MPDStatsValues MPDdata::getStatsValues() const { return statsValues_; }

// MPD song metadata
QString MPDdata::file() const { return songMetadataValues_.file; }

QString MPDdata::artist() const { return songMetadataValues_.artist; }

QString MPDdata::album() const { return songMetadataValues_.album; }

QString MPDdata::albumId() const { return songMetadataValues_.albumId; }

QString MPDdata::albumArtist() const { return songMetadataValues_.albumArtist; }

QString MPDdata::title() const { return songMetadataValues_.title; }

quint16 MPDdata::track() const { return songMetadataValues_.track; }

QString MPDdata::name() const { return songMetadataValues_.name; }

QString MPDdata::genre() const { return songMetadataValues_.genre; }

quint16 MPDdata::date() const { return songMetadataValues_.date; }

QString MPDdata::composer() const { return songMetadataValues_.composer; }

QString MPDdata::performer() const { return songMetadataValues_.performer; }

QString MPDdata::comment() const { return songMetadataValues_.comment; }

quint8 MPDdata::disc() const { return songMetadataValues_.disc; }

quint16 MPDdata::time() const { return songMetadataValues_.time; }

qint32 MPDdata::id() const { return songMetadataValues_.id; }

QString MPDdata::lastModified() const {
  return songMetadataValues_.lastModified;
}

uint MPDdata::pos() const { return songMetadataValues_.pos; }

MPDSongMetadata MPDdata::getSongMetadataValues() const {
  return songMetadataValues_;
}
