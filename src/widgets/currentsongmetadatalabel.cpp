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

   Description : Song Metadata Label
*/

#include <QDebug>
#include <QTimeLine>

#include "../core/application.h"
#include "../lib/mpdclient.h"
#include "../lib/mpddata.h"
#include "../tagger/currentartloader.h"
#include "currentsongmetadatalabel.h"

CurrentSongMetadataLabel::CurrentSongMetadataLabel(Application *app,
                                                   QWidget *parent)
    : QLabel(parent), app_(app), showHideAnimation_(new QTimeLine(500, this)) {
  setText("Todi");
  showHideAnimation_->setFrameRange(0, 255);
  connect(showHideAnimation_, &QTimeLine::frameChanged, this,
          &CurrentSongMetadataLabel::setOpacity);
  connect(
      app_->currentArtLoader(), &CurrentArtLoader::coverArtProcessed, [&]() {
        updateSongMetadata(app_->mpdClient()->getSharedMPDdataPtr()->title(),
                           app_->mpdClient()->getSharedMPDdataPtr()->album());
      });
}

CurrentSongMetadataLabel::~CurrentSongMetadataLabel() {}

void CurrentSongMetadataLabel::updateSongMetadataText(bool animate) {
  QFontMetrics fm(this->font());
  QString metadataHtml;
  if (!songMetaData_.first.isEmpty() || !songMetaData_.second.isEmpty()) {
    metadataHtml =
        QString("<p>%1<br/><i>%2</i></p>")
            .arg(fm.elidedText(songMetaData_.first, Qt::ElideRight, width()))
            .arg(fm.elidedText(songMetaData_.second, Qt::ElideRight, width()));
  } else {
    metadataHtml = QString("<p><i>%1</i></p>")
                       .arg(fm.elidedText("Todi", Qt::ElideRight, width()));
  }
  setText(metadataHtml);
  // while resizing we dont want animation
  if (animate) {
    startAnimation();
  }
}

void CurrentSongMetadataLabel::setOpacity(int value) {
  QString style =
      QString("SongMetadataLabel{color:rgba(200, 200, 200, %1)}").arg(value);
  setStyleSheet(style);
}

void CurrentSongMetadataLabel::showEvent(QShowEvent *) { startAnimation(); }

void CurrentSongMetadataLabel::hideEvent(QHideEvent *) {
  showHideAnimation_->stop();
}

void CurrentSongMetadataLabel::startAnimation() {
  if (isVisible()) {
    showHideAnimation_->setDirection(QTimeLine::Forward);
    showHideAnimation_->start();
  }
}

void CurrentSongMetadataLabel::updateSongMetadata(const QString arg1,
                                                  const QString arg2) {
  songMetaData_.first = arg1;
  songMetaData_.second = arg2;
  updateSongMetadataText();
}
