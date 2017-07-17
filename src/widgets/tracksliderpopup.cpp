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

#include "tracksliderpopup.h"

#include <QMouseEvent>
#include <QPainter>

const int TrackSliderPopup::kTextMargin = 4;
const int TrackSliderPopup::kPointLength = 8;
const int TrackSliderPopup::kPointWidth = 4;
const int TrackSliderPopup::kBorderRadius = 4;
const qreal TrackSliderPopup::kBlurRadius = 10.0;

TrackSliderPopup::TrackSliderPopup(QWidget* parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint),
      font_metrics_(fontMetrics()) {
  this->setAttribute(Qt::WA_TranslucentBackground, true);
  setAttribute(Qt::WA_TransparentForMouseEvents);
  setMouseTracking(true);
  setFocusPolicy(Qt::NoFocus);

  font_.setItalic(true);
  font_metrics_ = QFontMetrics(font_);
}

void TrackSliderPopup::SetText(const QString& text) {
  text_ = text;
  //if(font_metrics_.width(text_) != font_metrics_.width(text))
  UpdatePixmap();
}

void TrackSliderPopup::SetPopupPosition(const QPoint& pos) {
  pos_ = pos;
  UpdatePosition();
}

void TrackSliderPopup::paintEvent(QPaintEvent*) {
  QPainter p(this);
  p.drawPixmap(0, 0, pixmap_);
}

void TrackSliderPopup::UpdateBackground()
{

}

void TrackSliderPopup::UpdatePixmap() {
  const int text_width = font_metrics_.width(text_);
  const QRect text_rect1(static_cast<int>(kBlurRadius + kTextMargin),
                         static_cast<int>(kBlurRadius + kTextMargin),
                         text_width + 2, font_metrics_.height());

  const int bubble_bottom = text_rect1.bottom() + kTextMargin;
  const QRect total_rect(
      0, 0, static_cast<int>(text_rect1.right() + kBlurRadius + kTextMargin),
      static_cast<int>(kBlurRadius + bubble_bottom + kPointLength));
  const QRect bubble_rect(
      static_cast<int>(kBlurRadius), static_cast<int>(kBlurRadius),
      static_cast<int>(total_rect.width() - kBlurRadius * 2),
      static_cast<int>(bubble_bottom - kBlurRadius));

  if (background_cache_.size() != total_rect.size()) {
    const QColor highlight(
        palette().color(QPalette::Active, QPalette::Highlight));
    const QColor bg_color_1(highlight.lighter(110));
    const QColor bg_color_2(highlight.darker(120));

    QPolygon pointy;
    // bottom tooltip
    /*pointy << QPoint(total_rect.width() / 2 - kPointWidth,
                     text_rect2.top() + kTextMargin)
           << QPoint(total_rect.width() / 2, total_rect.top() - kBlurRadius)
           << QPoint(total_rect.width() / 2 + kPointWidth,
                     text_rect2.top() + kTextMargin);*/

    // top tooltip
    pointy << QPoint(total_rect.width() / 2 - kPointWidth, bubble_bottom)
           << QPoint(total_rect.width() / 2,
                     static_cast<int>(total_rect.bottom() - kBlurRadius))
           << QPoint(total_rect.width() / 2 + kPointWidth, bubble_bottom);

    QPolygon inner_pointy;
    inner_pointy << QPoint(pointy[0].x() + 1, pointy[0].y() - 1)
                 << QPoint(pointy[1].x(), pointy[1].y() - 1)
                 << QPoint(pointy[2].x() - 1, pointy[2].y() - 1);

    background_cache_ = QPixmap(total_rect.size());
    background_cache_.fill(Qt::transparent);
    QPainter p(&background_cache_);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::HighQualityAntialiasing);

    // Draw the shadow to a different image
    QImage blur_source(total_rect.size(), QImage::Format_ARGB32);
    blur_source.fill(Qt::transparent);

    QPainter blur_painter(&blur_source);
    blur_painter.setRenderHint(QPainter::Antialiasing);
    blur_painter.setRenderHint(QPainter::HighQualityAntialiasing);
    blur_painter.setBrush(bg_color_2);
    blur_painter.drawRoundedRect(bubble_rect, kBorderRadius, kBorderRadius);
    blur_painter.drawPolygon(pointy);

    // Fade the shadow out towards the bottom
    QLinearGradient fade_gradient(QPoint(0, bubble_bottom),
                                  QPoint(0, bubble_bottom + kPointLength));
    fade_gradient.setColorAt(0.0, QColor(255, 0, 0, 0));
    fade_gradient.setColorAt(1.0, QColor(255, 0, 0, 255));
    blur_painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    blur_painter.fillRect(total_rect, fade_gradient);
    blur_painter.end();

    p.save();
    qt_blurImage(&p, blur_source, kBlurRadius, true, false);
    p.restore();

    // Outer bubble
    p.setPen(Qt::NoPen);
    p.setBrush(bg_color_2);
    p.drawRoundedRect(bubble_rect, kBorderRadius, kBorderRadius);

    // Outer pointy
    p.drawPolygon(pointy);

    // Inner bubble
    p.setBrush(bg_color_1);
    p.drawRoundedRect(bubble_rect.adjusted(1, 1, -1, -1), kBorderRadius,
                      kBorderRadius);

    // Inner pointy
    p.drawPolygon(inner_pointy);
  }

  pixmap_ = QPixmap(total_rect.size());
  pixmap_.fill(Qt::transparent);
  QPainter p(&pixmap_);
  p.setRenderHint(QPainter::Antialiasing);
  p.setRenderHint(QPainter::HighQualityAntialiasing);

  // Background
  p.drawPixmap(total_rect.topLeft(), background_cache_);

  // Text
  p.setPen(palette().color(QPalette::HighlightedText));
  p.setFont(font_);
  p.drawText(text_rect1, Qt::AlignHCenter, text_);
  p.end();

  resize(pixmap_.size());
  UpdatePosition();
  update();
}

void TrackSliderPopup::UpdatePosition() {
  // works normal top
  move(static_cast<int>(pos_.x() - pixmap_.width() / 2),
       static_cast<int>(pos_.y() - pixmap_.height() + kBlurRadius));

  // works normal bottom
  // move(p.x() - width() / 2, p.y());
}
