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

#ifndef SONGMETADATALABEL_H
#define SONGMETADATALABEL_H

#include <QLabel>
#include <QTimer>

class Application;
class QTimeLine;
class QLine;

class CurrentSongMetadataLabel : public QLabel {
  Q_OBJECT
 public:
  CurrentSongMetadataLabel(Application *app, QWidget *parent = nullptr);
  ~CurrentSongMetadataLabel();
  void updateSongMetadata(const QString arg1, const QString arg2);

 public slots:
  void updateSongMetadataText(bool animate = true);
  void setOpacity(int value);

 protected:
  void showEvent(QShowEvent *);
  void hideEvent(QHideEvent *);

 private:
  void startAnimation() const;

  Application *app_;
  QPair<QString, QString> songMetaData_;
  QTimeLine *showHideAnimation_;
};

#endif  // SONGMETADATALABEL_H
