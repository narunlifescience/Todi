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

#ifndef METADATAWIDGET_H
#define METADATAWIDGET_H

#include <QTextEdit>
#include "mpdmodel.h"

class MetadataWidget : public QTextEdit {
  Q_OBJECT

 public:
  explicit MetadataWidget(QWidget *parent = nullptr);
  ~MetadataWidget();

 public slots:
  void setMetadata(MPDSongMetadata *songmetadata_);
};

#endif  // METADATAWIDGET_H
