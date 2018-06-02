#ifndef MPDLIBRARYMODEL_H
#define MPDLIBRARYMODEL_H

#include <QList>
#include <QVariant>

class MusicLibraryItem {
 public:
  enum class Type { TypeRoot, TypeArtist, TypeAlbum, TypeSong };
  MusicLibraryItem(const QString &data, Type type);
  virtual ~MusicLibraryItem();

  virtual MusicLibraryItem *child(int /*row*/) const { return nullptr; }
  virtual int childCount() const { return 0; }
  int columnCount() const { return 1; }
  QVariant data(int column) const;
  virtual int row() const { return 0; }
  virtual MusicLibraryItem *parent() const { return nullptr; }
  MusicLibraryItem::Type type() const;

 protected:
  Type m_type_;
  QString m_itemData_;
};

class MusicLibraryItemArtist;
class MusicLibraryItemSong;
class MusicLibraryItemRoot;
class MusicLibraryItemAlbum;

class MusicLibraryItemAlbum : public MusicLibraryItem {
 public:
  MusicLibraryItemAlbum(const QString &data,
                        MusicLibraryItem *parent = nullptr);
  ~MusicLibraryItemAlbum();

  void appendChild(MusicLibraryItem *const child);
  void insertChild(MusicLibraryItem *const child, const int place);

  MusicLibraryItem *child(int row) const;
  int childCount() const;
  int row() const;
  MusicLibraryItem *parent() const;
  void clearChildren();

 private:
  QList<MusicLibraryItemSong *> m_childItems;
  MusicLibraryItemArtist *const m_parentItem;

  friend class MusicLibraryItemSong;
};

class MusicLibraryItemArtist : public MusicLibraryItem {
 public:
  MusicLibraryItemArtist(const QString &data,
                         MusicLibraryItem *parent = nullptr);
  ~MusicLibraryItemArtist();

  void appendChild(MusicLibraryItem *const child);
  void insertChild(MusicLibraryItem *const child, const int place);

  MusicLibraryItem *child(int row) const;
  int childCount() const;
  int row() const;
  MusicLibraryItem *parent() const;
  void setParent(MusicLibraryItem *const parent);
  void clearChildren();

 private:
  QList<MusicLibraryItemAlbum *> m_childItems;
  MusicLibraryItemRoot *m_parentItem;

  friend class MusicLibraryItemAlbum;
};

class MusicLibraryItemRoot : public MusicLibraryItem {
 public:
  MusicLibraryItemRoot(const QString &data);
  ~MusicLibraryItemRoot();

  void appendChild(MusicLibraryItem *const child);
  void insertChild(MusicLibraryItem *const child, const int place);

  MusicLibraryItem *child(int row) const;
  int childCount() const;
  void clearChildren();

 private:
  QList<MusicLibraryItemArtist *> m_childItems;

  friend class MusicLibraryItemArtist;
};

class MusicLibraryItemSong : public MusicLibraryItem {
 public:
  MusicLibraryItemSong(const QString &data, MusicLibraryItem *parent = nullptr);
  ~MusicLibraryItemSong();

  int row() const;
  MusicLibraryItem *parent() const;
  const QString &file() const;
  void setFile(const QString &filename);
  void setTrack(quint32 track_nr);
  void setDisc(quint32 disc_nr);
  quint32 track() const;
  quint32 disc() const;

 private:
  quint32 m_track;
  QString m_file;
  quint32 m_disc;
  MusicLibraryItemAlbum *const m_parentItem;
};

#endif  // MPDLIBRARYMODEL_H
