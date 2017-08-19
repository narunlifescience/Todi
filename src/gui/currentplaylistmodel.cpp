#include "currentplaylistmodel.h"
#include <QDebug>
#include <QPixmap>

CurrentPlaylistModel::CurrentPlaylistModel(
    QList<MPDSongMetadata *> *playlistQueue, QObject *parent)
    : QAbstractListModel(parent),
      playlistQueue_(playlistQueue),
      song_id(-1),
      lastClickedRow(0) {}

CurrentPlaylistModel::~CurrentPlaylistModel() {}

QVariant CurrentPlaylistModel::headerData(int section,
                                          Qt::Orientation orientation,
                                          int role) const {
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
  if (!metadata) return QVariant();

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

void CurrentPlaylistModel::updateCurrentSong(quint32 id) { song_id = id; }

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
  emit playSong(index.row()-1);
}
