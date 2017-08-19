#ifndef CURRENTPLAYLISTVIEW_H
#define CURRENTPLAYLISTVIEW_H

#include <QStyledItemDelegate>

class CurrentPlaylistViewDeligate : public QStyledItemDelegate {
  Q_OBJECT
 public:
  explicit CurrentPlaylistViewDeligate(QObject *parent = nullptr);
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const;
  QSize sizeHint(const QStyleOptionViewItem &option,
                 const QModelIndex &index) const;
};

#endif  // CURRENTPLAYLISTVIEW_H
