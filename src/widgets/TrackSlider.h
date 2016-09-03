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

#ifndef TRACKSLIDER_H
#define TRACKSLIDER_H

#include <QObject>
#include <QSlider>

class TrackSliderPopup;

class TrackSlider : public QSlider {
  Q_OBJECT
 public:
  TrackSlider(Qt::Orientation orientation, QWidget* parent = nullptr);

 signals:
  void seekForward();
  void seekBackward();

 protected:
  void enterEvent(QEvent* event);
  void leaveEvent(QEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void wheelEvent(QWheelEvent* event);

 private:
  TrackSliderPopup* trackSliderPopup_;
};

#endif  // TRACKSLIDER_H
