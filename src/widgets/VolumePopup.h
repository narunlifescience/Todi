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

class QSlider;
class QGraphicsDropShadowEffect;

class VolumePopup : public QFrame {
  Q_OBJECT

 public:
  VolumePopup(QWidget *parent = nullptr);
  ~VolumePopup();
  QSize sizeHint() const;
  void setWidgetColor(QColor color);
  QColor getWidgetColor();
  QColor getWidgetShadowEffectColor();
  void setVolumeSlider(int vol);
  int getVolumeSlider();
  static const int blur_padding;

 protected:
  void paintEvent(QPaintEvent *);

 private:
  void drawSliderWidget();
  QSlider *slider;
  QPixmap pixmap_;
  static const int edge_curve;
  static const int widget_layout_padding;
  static QColor widget_color;
  static QColor widget_shadow_effect_color;

 signals:
  void volumePopupSliderChanged(quint8 value);
};

#endif  // VOLUMEPOPUP_H
