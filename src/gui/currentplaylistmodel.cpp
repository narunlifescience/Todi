#include "currentplaylistmodel.h"
#include <QDebug>
#include <QPixmap>

CurrentPlaylistModel::CurrentPlaylistModel(
    QList<MPDSongMetadata *> *playlistQueue, QObject *parent)
    : QAbstractListModel(parent),
      playlistQueue_(playlistQueue),
      song_id(-1),
      lastsong_id(-1) {}

CurrentPlaylistModel::~CurrentPlaylistModel() {}

QVariant CurrentPlaylistModel::headerData(int section,
                                          Qt::Orientation orientation,
                                          int role) const {
  Q_UNUSED(section)
  Q_UNUSED(orientation)
  Q_UNUSED(role)
  return QVariant();
}

int CurrentPlaylistModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return playlistQueue_->size();
}

QVariant CurrentPlaylistModel::data(const QModelIndex &index, int role) const {
  // invalid index
  if (!index.isValid()) {
    return QVariant();
  }
  // out of bound row value
  if (index.row() >= playlistQueue_->size()) return QVariant();

  MPDSongMetadata *metadata = playlistQueue_->at(index.row());
  if (!metadata) return QString("no metadata");

  switch (role) {
    case Qt::DisplayRole:
      return (metadata->title);
    case Qt::UserRole:
      return (metadata->album);
    case Qt::DecorationRole:
      return (QPixmap(":/icons/nocover.png"));
  }

  // in any other case
  return QVariant();
}

void CurrentPlaylistModel::updateCurrentSong(quint32 id) {
  lastsong_id = song_id;
  song_id = id;

  setData(index(song_id), QVariant(song_id), Qt::UserRole + 1);

  if (lastsong_id != -1)
    emit dataChanged(index(lastsong_id), index(lastsong_id));

  if (song_id != -1) emit dataChanged(index(song_id), index(song_id));
}

qint32 CurrentPlaylistModel::getRowId(qint32 row) const {
  if (playlistQueue_->size() <= row) {
    return -1;
  }
  return playlistQueue_->at(row)->id;
}

qint32 CurrentPlaylistModel::getRowPos(qint32 row) const {
  if (playlistQueue_->size() <= row) {
    return -1;
  }
  return playlistQueue_->at(row)->pos;
}

/* call this when the application starts & when thre is a change to current
   playlist outside Todi (probably from other clients). Avoid it otherwise for
   small operations */
void CurrentPlaylistModel::updateModel() {
  beginResetModel();
  endResetModel();
}

void CurrentPlaylistModel::doubleClicked(QModelIndex index) {
  // invalid index
  if (!index.isValid()) return;
  // out of bound row value
  if (index.row() >= playlistQueue_->size()) return;

  const uint position = playlistQueue_->at(index.row())->pos;
  emit playSong(position);
}
