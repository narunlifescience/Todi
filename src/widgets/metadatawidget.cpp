/* This file is part of Todi.

   Copyright 2018, Arun Narayanankutty <n.arun.lifescience@gmail.com>

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
*/

#include "metadatawidget.h"

MetadataWidget::MetadataWidget(QWidget *parent) : QTextEdit (parent) {
  setContentsMargins(1, 1, 1, 1);
  setReadOnly(true);
}

MetadataWidget::~MetadataWidget() {}

void MetadataWidget::setMetadata(MPDSongMetadata *songmetadata_) {
  if (!songmetadata_) return;

  QString metadatatext_ = QString("<table>");
  metadatatext_ +=
      QString(
          "<tr><td align=\"right\"><i>Title:</i></td><td>%1</td></tr>"
          "<tr><td align=\"right\"><i>Artist:</i></td><td>%2</td></tr>"
          "<tr><td align=\"right\"><i>Album:</i></td><td>%3</td></tr>"
          "<tr><td align=\"right\"><i>Album Artist:</i></td><td>%4</td></tr>"
          "<tr><td align=\"right\"><i>Year:</i></td><td>%5</td></tr>"
          "<tr><td align=\"right\"><i>Performer:</i></td><td>%6</td></tr>"
          "<tr><td align=\"right\"><i>Track No:</i></td><td>%7</td></tr>"
          "<tr><td align=\"right\"><i>Disc No:</i></td><td>%8</td></tr>"
          "<tr><td align=\"right\"><i>Genre:</i></td><td>%9</td></tr>"
          "<tr><td align=\"right\"><i>Album Id:</i></td><td>%10</td></tr>"
          "<tr><td align=\"right\"><i>Id :</i></td><td>%11</td></tr>"
          "<tr><td align=\"right\"><i>Pos:</i></td><td>%12</td></tr>"
          "<tr><td align=\"right\"><i>Last Modified:</i></td><td>%13</td></tr>"
          "<tr><td align=\"right\"><i>Time:</i></td><td>%14</td></tr>"
          "<tr><td align=\"right\"><i>File Name:</i></td><td>%15</td></tr>"
          "<tr><td align=\"right\"><i>Comment:</i></td><td>%16</td></tr>")
          .arg(songmetadata_->title)
          .arg(songmetadata_->artist)
          .arg(songmetadata_->album)
          .arg(songmetadata_->albumArtist)
          .arg(QString::number(songmetadata_->date))
          .arg(songmetadata_->performer)
          .arg(QString::number(songmetadata_->track))
          .arg(QString::number(songmetadata_->disc))
          .arg(songmetadata_->genre)
          .arg(songmetadata_->albumId)
          .arg(QString::number(songmetadata_->id))
          .arg(QString::number(songmetadata_->pos))
          .arg(songmetadata_->lastModified)
          .arg(QString::number(songmetadata_->time))
          .arg(songmetadata_->file)
          .arg(songmetadata_->comment);
  metadatatext_ += "</table>";
  setHtml(metadatatext_);
}
