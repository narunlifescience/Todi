#include "mpddata.h"
#include "commandcontroller.h"
#include "mpddataparser.h"

#include <QDebug>

const QByteArray MPDdata::statusCommand = "status";
const QByteArray MPDdata::statsCommand = "stats";

MPDdata::MPDdata(QObject *parent, std::shared_ptr<CommandController> cmdctrlr)
    : QObject(parent), cmdCtrlr_(cmdctrlr) {}

MPDdata::~MPDdata() {}

void MPDdata::updateMpdStatus(const MPDStatusValues &newStatusValues) {
  statusValues_ = std::move(newStatusValues);
}

void MPDdata::updateMpdStats(const MPDStatsValues &newStatsValues) {
  statsValues_ = newStatsValues;
}

void MPDdata::getMPDStatus() {
  QPair<QByteArray, bool> mpdStatus(cmdCtrlr_->sendCommand(statusCommand));
  if (mpdStatus.second) {
    statusValues_ = MPDdataParser::parseStatus(mpdStatus.first);
    emit MPDStatusUpdated();
  }
}

void MPDdata::getMPDStats() {
  QPair<QByteArray, bool> mpdStats(cmdCtrlr_->sendCommand(statsCommand));
  if (mpdStats.second) {
    statsValues_ = std::move(MPDdataParser::parseStats(mpdStats.first));
    emit MPDStatsUpdated();
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
