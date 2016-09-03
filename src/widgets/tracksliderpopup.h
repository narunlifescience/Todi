/* This file is part of Clementine.
   Copyright 2016, Arun Narayanankutty <n.arun.lifescience@gmail.com>
   Copyright 2011, David Sansome <me@davidsansome.com>

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TRACKSLIDERPOPUP_H
#define TRACKSLIDERPOPUP_H

#include <QWidget>

void qt_blurImage(QPainter* p, QImage& blurImage, qreal radius, bool quality,
                  bool alphaOnly, int transposed = 0);

class TrackSliderPopup : public QWidget {
  Q_OBJECT

 public:
  TrackSliderPopup(QWidget* parent = nullptr);

 public slots:
  void SetText(const QString& text);
  void SetPopupPosition(const QPoint& pos);

 protected:
  void paintEvent(QPaintEvent*);

 private:
  static const int kTextMargin;
  static const int kPointLength;
  static const int kPointWidth;
  static const int kBorderRadius;
  static const qreal kBlurRadius;

  void UpdatePixmap();
  void UpdatePosition();
  void SendMouseEventToParent(QMouseEvent* event);

 private:
  QString text_;
  QPoint pos_;

  QFont font_;
  QFontMetrics font_metrics_;
  QPixmap pixmap_;
  QPixmap background_cache_;
};

#endif  // TRACKSLIDERPOPUP_H
