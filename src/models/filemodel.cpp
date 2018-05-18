/*
 * Copyright (c) 2008 Sander Knopper (sander AT knopper DOT tk) and
 *                    Roeland Douma (roeland AT rullzer DOT com)
 *
 * This file is part of QtMPC.
 *
 * QtMPC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * QtMPC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QtMPC.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "filemodel.h"
#include "../lib/mpdfilemodel.h"

#include <QCommonStyle>
#include <QtDebug>

FileModel::FileModel(QObject *parent, RootItem *rootitem)
    : QAbstractItemModel(parent), rootItem(rootitem) {}

FileModel::~FileModel() {}

QModelIndex FileModel::index(int row, int column,
                             const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent)) return QModelIndex();

  const Item *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<Item *>(parent.internalPointer());

  Item *const childItem = parentItem->child(row);
  if (childItem) return createIndex(row, column, childItem);

  return QModelIndex();
}

QModelIndex FileModel::parent(const QModelIndex &index) const {
  if (!index.isValid()) return QModelIndex();

  const Item *const childItem = static_cast<Item *>(index.internalPointer());
  Item *const parentItem = childItem->parent();

  if (parentItem == rootItem) return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

QVariant FileModel::headerData(int /*section*/, Qt::Orientation /*orientation*/,
                               int /*role*/) const {
  return QVariant();
}

int FileModel::rowCount(const QModelIndex &parent) const {
  if (parent.column() > 0) return 0;

  const Item *parentItem;

  if (!parent.isValid()) {
    parentItem = rootItem;
  } else {
    parentItem = static_cast<Item *>(parent.internalPointer());
  }

  return parentItem->childCount();
}

int FileModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return static_cast<Item *>(parent.internalPointer())->columnCount();
  else
    return rootItem->columnCount();
}

QVariant FileModel::data(const QModelIndex &index, int role) const {
  // invalid index
  if (!index.isValid()) {
    return QVariant();
  }

  if (role != Qt::DisplayRole && role != Qt::DecorationRole) return QVariant();

  Item *item = static_cast<Item *>(index.internalPointer());

  if (role == Qt::DecorationRole) {
    QCommonStyle style;
    if (item->type() == Item::Type::TypeFolder) {
      return style.standardIcon(QStyle::SP_DirIcon);
    } else if (item->type() == Item::Type::TypeFile) {
      return style.standardIcon(QStyle::SP_FileIcon);
    }
  } else {
    return item->data(index.column());
  }

  return QVariant();
}

void FileModel::ViewUpdated() {
  beginResetModel();
  endResetModel();
}
