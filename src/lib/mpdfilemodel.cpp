#include "mpdfilemodel.h"

Item::Item(const QString name, Type type) : name_(name), type_(type) {}

Item::~Item() {}

FolderItem::FolderItem(const QString name, Item *parent)
    : Item(name, Item::Type::TypeFolder), parentItem_(parent) {}

FolderItem::~FolderItem() { qDeleteAll(childItems_); }

int FolderItem::row() const {
  switch (parentItem_->type()) {
    case Item::Type::TypeRoot:
      return static_cast<RootItem *>(parentItem_)
          ->childItems_.indexOf(const_cast<FolderItem *>(this));
    case Item::Type::TypeFolder:
      return static_cast<FolderItem *>(parentItem_)
          ->childItems_.indexOf(const_cast<FolderItem *>(this));
    default:
      break;
  }

  return 0;
}

Item *FolderItem::parent() const { return parentItem_; }

int FolderItem::childCount() const { return childItems_.count(); }

Item *FolderItem::createDirectory(const QString dirName) {
  FolderItem *dir = new FolderItem(dirName, this);
  childItems_.append(dir);

  return dir;
}

Item *FolderItem::insertFile(const QString fileName) {
  FileItem *file = new FileItem(fileName, this);
  childItems_.append(file);

  return file;
}

Item *FolderItem::child(int row) const { return childItems_.value(row); }

FileItem::FileItem(const QString name, Item *parent)
    : Item(name, Item::Type::TypeFile), parentItem_(parent) {}

FileItem::~FileItem() {}

int FileItem::row() const {
  switch (parentItem_->type()) {
    case Item::Type::TypeRoot:
      return static_cast<RootItem *>(parentItem_)
          ->childItems_.indexOf(const_cast<FileItem *>(this));
    case Item::Type::TypeFolder:
      return static_cast<FolderItem *>(parentItem_)
          ->childItems_.indexOf(const_cast<FileItem *>(this));
    default:
      break;
  }

  return 0;
}

Item *FileItem::parent() const { return parentItem_; }

QString FileItem::fileName() {
  QString filename = name_;

  Item *current = this->parent();
  while (current->type() != Item::Type::TypeRoot) {
    filename.prepend("/");
    filename.prepend(current->name());
    current = current->parent();
  }
  return filename;
}

RootItem::RootItem(const QString name) : Item(name, Item::Type::TypeRoot) {}

RootItem::~RootItem() { qDeleteAll(childItems_); }

int RootItem::childCount() const { return childItems_.count(); }

Item *RootItem::createDirectory(const QString dirName) {
  FolderItem *dir = new FolderItem(dirName, this);
  childItems_.append(dir);

  return dir;
}

Item *RootItem::insertFile(const QString fileName) {
  FileItem *file = new FileItem(fileName, this);
  childItems_.append(file);

  return file;
}

Item *RootItem::child(int row) const { return childItems_.value(row); }

void RootItem::clear() {
  qDeleteAll(childItems_.begin(), childItems_.end());
  childItems_.clear();
}
