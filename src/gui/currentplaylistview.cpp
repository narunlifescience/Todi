#include "currentplaylistview.h"

#include <QPainter>

CurrentPlaylistViewDeligate::CurrentPlaylistViewDeligate(QObject *parent)
    : QStyledItemDelegate(parent) {}

void CurrentPlaylistViewDeligate::paint(QPainter *painter,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const {
  if (index.row() == 0) {
    return;
  }

  QRect r = option.rect;
  QColor text_color = QColor(200, 200, 200, 200);

  QFont normal_font = option.font;
  normal_font.setItalic(false);
  QFont description_font = normal_font;
  description_font.setItalic(true);
  QFontMetrics fm(normal_font);

  if (option.state & QStyle::State_Selected) {
    QLinearGradient gradientSelected(r.left(), r.top(), r.left(),
                                     r.height() + r.top());
    gradientSelected.setColorAt(0.0, QColor::fromRgb(119, 213, 247, 150));
    gradientSelected.setColorAt(0.9, QColor::fromRgb(27, 134, 183, 150));
    gradientSelected.setColorAt(1.0, QColor::fromRgb(0, 120, 174, 150));
    painter->setBrush(gradientSelected);
    painter->setPen(Qt::NoPen);
    painter->drawRect(r);

    // BORDER
    /*painter->setPen(lineMarkedPen);
    painter->drawLine(r.topLeft(), r.topRight());
    painter->drawLine(r.topRight(), r.bottomRight());
    painter->drawLine(r.bottomLeft(), r.bottomRight());
    painter->drawLine(r.topLeft(), r.bottomLeft());*/

    // painter->setPen(fontMarkedPen);

  } else {
    // BACKGROUND
    // ALTERNATING COLORS
    // painter->setBrush((index.row() % 2) ? Qt::white : QColor(252, 252, 252));
    // painter->drawRect(r);

    // BORDER
    QColor line_color = text_color;
    QLinearGradient grad_color(r.bottomLeft(), r.bottomRight());
    const double fade_start_end = (r.width() / 3.0) / r.width();
    line_color.setAlphaF(0.0);
    grad_color.setColorAt(0, line_color);
    line_color.setAlphaF(0.3);
    grad_color.setColorAt(fade_start_end, line_color);
    grad_color.setColorAt(1.0 - fade_start_end, line_color);
    line_color.setAlphaF(0.0);
    grad_color.setColorAt(1, line_color);
    painter->setPen(QPen(grad_color, 1));
    painter->drawLine(r.bottomLeft(), r.bottomRight());
  }

  int imageSpace = 10;
  // GET TITLE, DESCRIPTION AND ICON
  QIcon icon = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
  QString title = fm.elidedText(index.data(Qt::DisplayRole).toString(),
                                Qt::ElideRight, r.width() - imageSpace - 50);
  QString description =
      fm.elidedText(index.data(Qt::UserRole).toString(), Qt::ElideRight,
                    r.width() - imageSpace - 50);

  if (!icon.isNull()) {
    // ICON
    r = option.rect.adjusted(5, 10, -10, -10);
    icon.paint(painter, r, Qt::AlignVCenter | Qt::AlignLeft);
    imageSpace = 55;
  }

  // TITLE
  r = option.rect.adjusted(imageSpace, 0, -10, -30);
  painter->setFont(normal_font);
  painter->setPen(QColor(Qt::white));
  painter->drawText(r.left(), r.top(), r.width(), r.height(),
                    Qt::AlignBottom | Qt::AlignLeft, title, &r);

  // DESCRIPTION
  r = option.rect.adjusted(imageSpace, 30, -10, 0);
  painter->setFont(description_font);
  painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignLeft,
                    description, &r);
}

QSize CurrentPlaylistViewDeligate::sizeHint(const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const {
  // first row is always header
  Q_UNUSED(option)
  if (index.row() == 0) {
    return QSize(0, 0);
  } else {
    return QSize(20, 60);
  }
}