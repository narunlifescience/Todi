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

   Description : Main player
*/

#ifndef PLAYER_H
#define PLAYER_H

#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QWidget>
#include <memory>
#include <QSystemTrayIcon>
#include <QVector>
#include <QTimer>

//#include "TrackSlider.h"
#include "mpdmodel.h"

class Ui_Player;
class QHBoxLayout;
class QVBoxLayout;

class MPDClient;
class MPDdata;
class PlaybackController;
class TrackSlider;

class Player : public QWidget {
  Q_OBJECT

 public:
  Player(QWidget *parent = nullptr);
  ~Player();

 protected:
  QSize sizeHint() const;
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void leaveEvent(QEvent *);
  void enterEvent(QEvent *);
  void resizeEvent(QResizeEvent *);

 private:
  Ui_Player *ui_;
  QPoint dragPosition;

  std::unique_ptr <MPDClient> mpdClient_;
  std::shared_ptr<MPDdata> dataAccess_;
  std::shared_ptr<PlaybackController> playbackCtrlr_;

  MPDPlaybackState lastState;
  qint32 lastSongId;

  int fetchStatsFactor;
  int nowPlayingFactor;
  QTimer statusTimer;

  bool draggingPositionSlider;
  QLabel bitrateLabel;

  QWidget *widget;
  QPushButton *close_pushButton;
  QPushButton *expand_collapse_PushButton;
  QPushButton *previous_pushButton;
  QPushButton *play_pause_pushButton;
  QPushButton *next_pushButton;
  QPushButton *volume_pushButton;
  QPushButton *search_pushButton;
  QPushButton *playlist_pushButton;
  TrackSlider *track_slider;
  QLabel *timer_label;
  QLabel *albumcover_label;
  QLabel *songMetadata_label;
  QFrame *volume_slider_frame;
  QSlider *volume_slider;
  bool resize_status;

  QSystemTrayIcon *trayIcon;
  QMenu *trayIconMenu;

  int showMpdConnectionDialog();
  bool setupTrayIcon();
  void setIconProgress(int progress);

 private slots:
  void expandCollapse();
  void showVolumeSlider();
  void updateStats();
  void updateStatus();
  void playPauseTrack();
  void stopTrack();
  void positionSliderPressed();
  void setPosition();
  void seekBackward();
  void seekForward();
  void positionSliderReleased();
  void setAlbumCover(QImage, QString, QString);
  void trayIconClicked(QSystemTrayIcon::ActivationReason reason);

 signals:
  void submitSong();
  void startSong();
  void stopSong();
  void pauseSong();
  void resumeSong();
  void nowPlaying();
};

#endif  // PLAYER_H
