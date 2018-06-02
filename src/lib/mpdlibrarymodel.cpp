#include "mpdlibrarymodel.h"

MusicLibraryItem::MusicLibraryItem(const QString &data, Type type)
    : m_type_(type), m_itemData_(data) {}

MusicLibraryItem::~MusicLibraryItem() {}

QVariant MusicLibraryItem::data(int /*column*/) const { return m_itemData_; }

MusicLibraryItem::Type MusicLibraryItem::type() const { return m_type_; }

MusicLibraryItemAlbum::MusicLibraryItemAlbum(const QString &data,
                                             MusicLibraryItem *parent)
    : MusicLibraryItem(data, MusicLibraryItem::Type::TypeAlbum),
      m_parentItem(static_cast<MusicLibraryItemArtist *>(parent)) {}

MusicLibraryItemAlbum::~MusicLibraryItemAlbum() { qDeleteAll(m_childItems); }

void MusicLibraryItemAlbum::appendChild(MusicLibraryItem *const item) {
  m_childItems.append(static_cast<MusicLibraryItemSong *>(item));
}

void MusicLibraryItemAlbum::insertChild(MusicLibraryItem *const child,
                                        const int place) {
  m_childItems.insert(place, static_cast<MusicLibraryItemSong *>(child));
}

MusicLibraryItem *MusicLibraryItemAlbum::child(int row) const {
  return m_childItems.value(row);
}

int MusicLibraryItemAlbum::childCount() const { return m_childItems.count(); }

MusicLibraryItem *MusicLibraryItemAlbum::parent() const { return m_parentItem; }

int MusicLibraryItemAlbum::row() const {
  return m_parentItem->m_childItems.indexOf(
      const_cast<MusicLibraryItemAlbum *>(this));
}

void MusicLibraryItemAlbum::clearChildren() { qDeleteAll(m_childItems); }

MusicLibraryItemArtist::MusicLibraryItemArtist(const QString &data,
                                               MusicLibraryItem *parent)
    : MusicLibraryItem(data, MusicLibraryItem::Type::TypeArtist),
      m_parentItem(static_cast<MusicLibraryItemRoot *>(parent)) {}

MusicLibraryItemArtist::~MusicLibraryItemArtist() { qDeleteAll(m_childItems); }

void MusicLibraryItemArtist::appendChild(MusicLibraryItem *const item) {
  m_childItems.append(static_cast<MusicLibraryItemAlbum *>(item));
}

void MusicLibraryItemArtist::insertChild(MusicLibraryItem *const child,
                                         const int place) {
  m_childItems.insert(place, static_cast<MusicLibraryItemAlbum *>(child));
}

MusicLibraryItem *MusicLibraryItemArtist::child(int row) const {
  return m_childItems.value(row);
}

int MusicLibraryItemArtist::childCount() const { return m_childItems.count(); }

MusicLibraryItem *MusicLibraryItemArtist::parent() const {
  return m_parentItem;
}

void MusicLibraryItemArtist::setParent(MusicLibraryItem *const parent) {
  m_parentItem = static_cast<MusicLibraryItemRoot *>(parent);
}

int MusicLibraryItemArtist::row() const {
  return m_parentItem->m_childItems.indexOf(
      const_cast<MusicLibraryItemArtist *>(this));
}

void MusicLibraryItemArtist::clearChildren() { qDeleteAll(m_childItems); }

MusicLibraryItemRoot::MusicLibraryItemRoot(const QString &data)
    : MusicLibraryItem(data, MusicLibraryItem::Type::TypeRoot) {}

MusicLibraryItemRoot::~MusicLibraryItemRoot() { qDeleteAll(m_childItems); }

void MusicLibraryItemRoot::appendChild(MusicLibraryItem *const item) {
  m_childItems.append(static_cast<MusicLibraryItemArtist *>(item));
}

void MusicLibraryItemRoot::insertChild(MusicLibraryItem *const child,
                                       const int place) {
  m_childItems.insert(place, static_cast<MusicLibraryItemArtist *>(child));
}

MusicLibraryItem *MusicLibraryItemRoot::child(int row) const {
  return m_childItems.value(row);
}

int MusicLibraryItemRoot::childCount() const { return m_childItems.count(); }

void MusicLibraryItemRoot::clearChildren() { qDeleteAll(m_childItems); }

MusicLibraryItemSong::MusicLibraryItemSong(const QString &data,
                                           MusicLibraryItem *parent)
    : MusicLibraryItem(data, MusicLibraryItem::Type::TypeSong),
      m_track(0),
      m_disc(0),
      m_parentItem(static_cast<MusicLibraryItemAlbum *>(parent)) {}

MusicLibraryItemSong::~MusicLibraryItemSong() {}

MusicLibraryItem *MusicLibraryItemSong::parent() const { return m_parentItem; }

int MusicLibraryItemSong::row() const {
  return m_parentItem->m_childItems.indexOf(
      const_cast<MusicLibraryItemSong *>(this));
}

const QString &MusicLibraryItemSong::file() const { return m_file; }

void MusicLibraryItemSong::setFile(const QString &filename) {
  m_file = filename;
}

void MusicLibraryItemSong::setTrack(quint32 track_nr) { m_track = track_nr; }

void MusicLibraryItemSong::setDisc(quint32 disc_nr) { m_disc = disc_nr; }

quint32 MusicLibraryItemSong::track() const { return m_track; }

quint32 MusicLibraryItemSong::disc() const { return m_disc; }
