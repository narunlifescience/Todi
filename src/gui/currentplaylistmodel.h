#ifndef CURRENTPLAYLISTMODEL_H
#define CURRENTPLAYLISTMODEL_H

#include <QAbstractListModel>

#include "../lib/mpdmodel.h"

class CurrentPlaylistModel : public QAbstractListModel {
  Q_OBJECT
 public:
  explicit CurrentPlaylistModel(
      QList<MPDSongMetadata *> *playlistQueue = nullptr,
      QObject *parent = nullptr);
  ~CurrentPlaylistModel();
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role) const;
  void updateCurrentSong(quint32 id);
  qint32 getRowId(qint32 row) const;
  qint32 getRowPos(qint32 row) const;
  qint32 songIdToRow(qint32 id) const;
  qint32 getCurrentSongId() const { return song_id; }

 signals:
  void playSong(quint32 song);

 public slots:
  void updateModel();
  void doubleClicked(QModelIndex index);

 private:
  const QList<MPDSongMetadata *> *playlistQueue_;
  qint32 song_id;
  qint32 lastsong_id;
};

#endif  // CURRENTPLAYLISTMODEL_H
