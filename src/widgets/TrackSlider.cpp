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

#include <stdio.h>
#include <QMouseEvent>
#include <QStyle>
#include <QStyleOptionSlider>

#include "TrackSlider.h"
#include "tracksliderpopup.h"

TrackSlider::TrackSlider(Qt::Orientation orientation, QWidget *parent)
    : QSlider(orientation, parent),
      trackSliderPopup_(new TrackSliderPopup(window())) {
  setMouseTracking(true);
}

void TrackSlider::enterEvent(QEvent *event) {
  QSlider::enterEvent(event);
  if (isEnabled()) {
    trackSliderPopup_->show();
  }
}

void TrackSlider::leaveEvent(QEvent *event) {
  QSlider::leaveEvent(event);
  trackSliderPopup_->hide();
}

void TrackSlider::mousePressEvent(QMouseEvent *event) {
  Qt::MouseButton new_button = event->button();
  if (event->button() == Qt::LeftButton) {
    uint abs_buttons = static_cast<uint>(
        style()->styleHint(QStyle::SH_Slider_AbsoluteSetButtons));
    if (abs_buttons & Qt::LeftButton)
      new_button = Qt::LeftButton;
    else if (abs_buttons & Qt::MidButton)
      new_button = Qt::MidButton;
    else if (abs_buttons & Qt::RightButton)
      new_button = Qt::RightButton;
  }

  QMouseEvent new_event(event->type(), event->pos(), new_button, new_button,
                        event->modifiers());
  QSlider::mousePressEvent(&new_event);

  if (new_event.isAccepted()) event->accept();
}

void TrackSlider::mouseMoveEvent(QMouseEvent *event) {
  QSlider::mouseMoveEvent(event);

  // Borrowed from QSliderPrivate::pixelPosToRangeValue
  QStyleOptionSlider opt;
  initStyleOption(&opt);
  QRect gr = style()->subControlRect(QStyle::CC_Slider, &opt,
                                     QStyle::SC_SliderGroove, this);
  QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt,
                                     QStyle::SC_SliderHandle, this);

  int slider_length = sr.width();
  int slider_min = gr.x();
  int slider_max = gr.right() - slider_length + 1;

  int seconds = QStyle::sliderValueFromPosition(
      minimum(), maximum() / 1, event->x() - slider_length / 2 - slider_min + 1,
      slider_max - slider_min);
  int hours = seconds / (60 * 60);

  if (hours) {
    trackSliderPopup_->SetText(QString().sprintf(
        "%d:%02d:%02d", hours, (seconds / 60) % 60, seconds % 60));
  } else {
    trackSliderPopup_->SetText(
        QString().sprintf("%d:%02d", (seconds / 60) % 60, seconds % 60));
  }

  trackSliderPopup_->SetPopupPosition(
      mapTo(window(), QPoint(event->x(), rect().center().y())));
}

void TrackSlider::wheelEvent(QWheelEvent *event) {
  (event->delta() > 0) ? emit seekForward() : emit seekBackward();
  event->accept();
}
