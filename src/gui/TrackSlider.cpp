/* This file is part of Todi.

   Copyright 2016, Arun Narayanankutty <n.arun.lifescience@gmail.com>

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

   Description : Track Slider
*/

#include <QMouseEvent>
#include <QStyle>
#include <QStyleOptionSlider>

#include "TrackSlider.h"

TrackSlider::TrackSlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent) {}

TrackSlider::~TrackSlider() {}

void TrackSlider::mousePressEvent(QMouseEvent *event) {
  {
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt,
                                       QStyle::SC_SliderHandle, this);

    if (event->button() == Qt::LeftButton && !sr.contains(event->pos())) {
      int newVal;
      double halfHandleWidth = (0.5 * sr.width()) + 0.5;
      int adaptedPosX = event->x();
      if (adaptedPosX < halfHandleWidth)
        adaptedPosX = static_cast<int>(halfHandleWidth);
      if (adaptedPosX > width() - halfHandleWidth)
        adaptedPosX = static_cast<int>(width() - halfHandleWidth);
      double newWidth = (width() - halfHandleWidth) - halfHandleWidth;
      double normalizedPosition = (adaptedPosX - halfHandleWidth) / newWidth;
      newVal = static_cast<int>(minimum() +
                                ((maximum() - minimum()) * normalizedPosition));

      if (invertedAppearance()) {
        setValue(maximum() - newVal);
      } else {
        setValue(newVal);
      }

      event->accept();
    }
    QSlider::mousePressEvent(event);
  }
}
