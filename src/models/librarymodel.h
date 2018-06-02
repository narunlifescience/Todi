#ifndef LIBRARYMODEL_H
#define LIBRARYMODEL_H

#include <QAbstractItemModel>
#include <QDateTime>
#include <QMimeData>
#include <QSettings>

class MusicLibraryItemAlbum;
class MusicLibraryItemArtist;
class MusicLibraryItemRoot;

class LibraryModel : public QAbstractItemModel {
  Q_OBJECT

 public:
  LibraryModel(QObject *parent = nullptr);
  ~LibraryModel();
  QModelIndex index(int, int, const QModelIndex & = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &) const;
  QVariant data(const QModelIndex &, int) const;
  bool fromXML(const QDateTime db_update);

  Qt::ItemFlags flags(const QModelIndex &index) const;
  QMimeData *mimeData(const QModelIndexList &indexes) const;

 public slots:
  void updateLibrary(QList<MusicLibraryItemArtist *> *items,
                     QDateTime db_update = QDateTime(), bool fromFile = false);

 signals:
  void xmlWritten(QDateTime db_update);

 private:
  const MusicLibraryItemRoot *rootItem;
  QSettings settings;
  QStringList sortAlbumTracks(const MusicLibraryItemAlbum *album) const;

  void toXML(const QDateTime db_update);
};

#endif  // LIBRARYMODEL_H
