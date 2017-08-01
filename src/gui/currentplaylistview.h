#ifndef CURRENTPLAYLISTVIEW_H
#define CURRENTPLAYLISTVIEW_H

#include <QStyledItemDelegate>
#include <QListView>

class CurrentPlaylistViewDeligate : public QStyledItemDelegate {
  Q_OBJECT
 public:
  explicit CurrentPlaylistViewDeligate(QObject *parent = nullptr);
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const;
  QSize sizeHint(const QStyleOptionViewItem &option,
                 const QModelIndex &index) const;
};

class CurrentPlaylistView : public QListView {
  Q_OBJECT
 public:
  explicit CurrentPlaylistView(QWidget *parent = nullptr);

private:
  void customStylesheet();
};

#endif  // CURRENTPLAYLISTVIEW_H
