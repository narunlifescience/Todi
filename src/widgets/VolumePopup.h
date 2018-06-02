/* This file is part of Todi.

   Copyright 2017, Arun Narayanankutty <n.arun.lifescience@gmail.com>

   Todi is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   Todi is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with Todi.  If not, see <http://www.gnu.org/licenses/>.

   Description : Volume Popup widget
*/

#ifndef VOLUMEPOPUP_H
#define VOLUMEPOPUP_H

#include <QFrame>
#include <QSlider>

class VolumeSlider : public QSlider {
  Q_OBJECT
 public:
  VolumeSlider(Qt::Orientation orientation, QWidget* parent = nullptr);

 signals:
  void deltaIncreaseVolume();
  void deltaDecreaseVolume();

 protected:
  void mousePressEvent(QMouseEvent* event);
  void wheelEvent(QWheelEvent* event);
};

class VolumePopup : public QFrame {
  Q_OBJECT

 public:
  VolumePopup(QWidget* parent = nullptr);
  ~VolumePopup();
  QSize sizeHint() const;
  void setWidgetColor(QColor color);
  QColor getWidgetColor();
  void redrawSliderWidget();
  void setVolumeSlider(int vol);
  int getVolumeSlider();
  static int blur_padding;
  static int edge_curve;
  static int widget_layout_padding;

 public slots:
  void setVolumeSliderStylesheet(QString stylesheet);

 protected:
  void paintEvent(QPaintEvent*);
  void wheelEvent(QWheelEvent* event);

 private slots:
  void deltaIncreaseVolume();
  void deltaDecreaseVolume();

 private:
  void drawSliderWidget();
  VolumeSlider* slider_;
  QPixmap pixmap_;
  static QColor widget_color;
  static const int delta_volume;

 signals:
  void volumePopupSliderChanged(quint8 value);
};

#endif  // VOLUMEPOPUP_H
