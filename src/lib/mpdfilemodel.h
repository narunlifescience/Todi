#ifndef MPDFILEMODEL_H
#define MPDFILEMODEL_H

#include <QString>
#include <QVariant>

class Item {
 public:
  enum class Type { TypeRoot, TypeFolder, TypeFile };

  Item(const QString name, Type type);
  virtual ~Item();

  virtual int row() const { return 0; }
  virtual Item* parent() const { return nullptr; }
  virtual int childCount() const { return 0; }
  int columnCount() const { return 1; }
  QVariant data(int) const { return name_; }
  virtual Item* child(int /*row*/) const { return nullptr; }
  QString name() { return name_; }
  Item::Type type() const { return type_; }
  virtual QString fileName() { return QString(); }

 protected:
  QString name_;
  Type type_;
};

class FolderItem : public Item {
 public:
  FolderItem(const QString name, Item* parent = nullptr);
  ~FolderItem();

  Item* createDirectory(const QString dirName);
  Item* insertFile(const QString fileName);

  int row() const;
  Item* parent() const;
  int childCount() const;
  Item* child(int row) const;

 private:
  Item* const parentItem_;
  QList<Item*> childItems_;

  friend class FileItem;
};

class FileItem : public Item {
 public:
  FileItem(const QString name, Item* parent = nullptr);
  ~FileItem();

  int row() const;
  Item* parent() const;
  QString fileName();

 private:
  Item* const parentItem_;
};

class RootItem : public Item {
 public:
  RootItem(const QString name);
  ~RootItem();

  Item* createDirectory(const QString dirName);
  Item* insertFile(const QString fileName);

  int childCount() const;
  Item* child(int row) const;
  void clear();

 private:
  QList<Item*> childItems_;

  friend class FolderItem;
  friend class FileItem;
};

#endif
