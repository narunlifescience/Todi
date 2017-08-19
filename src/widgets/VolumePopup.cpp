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

#include "VolumePopup.h"

#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QPainter>
#include <QSlider>

const int VolumePopup::blur_padding = 4;
const int VolumePopup::edge_curve = 6;
const int VolumePopup::widget_layout_padding = 6;
QColor VolumePopup::widget_color = QColor(20, 20, 20, 200);
QColor VolumePopup::widget_shadow_effect_color = QColor(181, 185, 190);

VolumePopup::VolumePopup(QWidget *parent)
    : QFrame(parent,
             Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::Popup),
      slider(new QSlider(Qt::Horizontal)) {
  this->setAttribute(Qt::WA_TranslucentBackground, true);
  QGraphicsDropShadowEffect *shadow_effect =
      new QGraphicsDropShadowEffect(this);
  shadow_effect->setColor(widget_shadow_effect_color);
  shadow_effect->setBlurRadius(blur_padding);
  shadow_effect->setXOffset(0);
  shadow_effect->setYOffset(0);
  setGraphicsEffect(shadow_effect);

  this->setFixedHeight(sizeHint().height());
  this->setFixedWidth(sizeHint().width());
  drawSliderWidget();

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(
      widget_layout_padding + blur_padding,
      widget_layout_padding + blur_padding,
      widget_layout_padding + blur_padding,
      (height() / 6) + blur_padding + widget_layout_padding);
  layout->addWidget(slider);

  slider->setMaximum(0);
  slider->setMaximum(100);

  connect(slider, &QSlider::valueChanged, this,
          &VolumePopup::volumePopupSliderChanged);
}

VolumePopup::~VolumePopup() {}

QSize VolumePopup::sizeHint() const { return QSize(100, 50); }

void VolumePopup::setWidgetColor(QColor color) { widget_color = color; }

QColor VolumePopup::getWidgetColor() { return widget_color; }

QColor VolumePopup::getWidgetShadowEffectColor() {
  return widget_shadow_effect_color;
}

void VolumePopup::setVolumeSlider(int vol) { slider->setValue(vol); }

int VolumePopup::getVolumeSlider() { return slider->value(); }

void VolumePopup::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.drawPixmap(0, 0, pixmap_);
}

void VolumePopup::drawSliderWidget() {
  int x1 = 0 + blur_padding;
  int y1 = 0 + blur_padding;
  int x2 = width() - blur_padding;
  int y2 = (height() - blur_padding) - (height() / 6);

  QPainterPath paintPath;
  paintPath.moveTo(QPoint(x1 + edge_curve, y1));
  paintPath.lineTo(QPoint(x2 - edge_curve, y1));
  paintPath.cubicTo(QPoint(x2 - edge_curve, y1), QPoint(x2, y1),
                    QPoint(x2, y1 + edge_curve));
  paintPath.lineTo(QPoint(x2, y2 - edge_curve));
  paintPath.cubicTo(QPoint(x2, y2 - edge_curve), QPoint(x2, y2),
                    QPoint(x2 - edge_curve, y2));

  paintPath.lineTo(QPoint((width() / 2) + (blur_padding * 2), y2));
  paintPath.lineTo(QPoint(width() / 2, height() - blur_padding));
  paintPath.lineTo(QPoint((width() / 2) - (blur_padding * 2), y2));
  paintPath.lineTo(QPoint(x1 + edge_curve, y2));

  paintPath.cubicTo(QPoint(x1 + edge_curve, y2), QPoint(x1, y2),
                    QPoint(x1, y2 - edge_curve));
  paintPath.lineTo(QPoint(x1, y1 + edge_curve));
  paintPath.cubicTo(QPoint(x1, y1 + edge_curve), QPoint(x1, y1),
                    QPoint(x1 + edge_curve, y1));

  pixmap_ = QPixmap(this->sizeHint());
  pixmap_.fill(Qt::transparent);
  QPainter painter(&pixmap_);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setBrush(QBrush(widget_color));
  painter.setPen(Qt::NoPen);
  painter.drawPath(paintPath);
  update();
}
