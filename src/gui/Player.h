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
#include <QTimer>
#include <QVector>
#include <QWidget>
#include <memory>

#include "mpdmodel.h"

class Ui_Player;
class QHBoxLayout;
class QGridLayout;
class QStackedWidget;

class Application;
class SystemTrayIcon;
class MPDClient;
class MPDdata;
class PlaybackController;
class PlaybackOptionsController;
class TrackSlider;
class VolumePopup;
class CurrentArtLoader;
class CurrentSongMetadataLabel;
class MetadataWidget;
class CurrentCoverArtLabel;
class CurrentPlaylistModel;
class FileModel;
class FancyTabWidget;
class ConsoleWidget;
class Theme;
class QListView;
class QTreeView;

class Player : public QWidget {
  Q_OBJECT

 public:
  Player(Application *app, QWidget *parent = nullptr);
  ~Player();

 protected:
  QSize sizeHint() const;
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void leaveEvent(QEvent *);
  void enterEvent(QEvent *);
  void resizeEvent(QResizeEvent *);
  bool eventFilter(QObject *target, QEvent *event);

 private:
  Ui_Player *ui_;
  Application *app_;
  Theme *theme_;
  QPoint dragPosition;

  MPDClient *mpdClient_;
  std::shared_ptr<MPDdata> dataAccess_;
  std::shared_ptr<PlaybackController> playbackCtrlr_;
  std::shared_ptr<PlaybackOptionsController> playbackOptionsCtrlr_;
  CurrentArtLoader *currentArtLoader_;

  MPDPlaybackState lastState;
  qint32 lastSongId;
  quint32 lastPlaylist;

  int fetchStatsFactor;
  int nowPlayingFactor;
  QTimer statusTimer;

  bool draggingPositionSlider;
  QLabel bitrateLabel;

  QWidget *mainWidget;
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
  CurrentCoverArtLabel *currentCoverArt_label;
  CurrentSongMetadataLabel *currentSongMetadata_label;
  VolumePopup *volume_popup;
  QStackedWidget *stack_widget;
  MetadataWidget *metadata_widget;
  FancyTabWidget *fancy_tab_widget;
  ConsoleWidget *console_widget_;
  QListView *playlist_view;
  QTreeView *folder_view_;
  CurrentPlaylistModel *currentPlaylistModel_;
  FileModel *filemodel_;
  bool resize_status;
  SystemTrayIcon *trayIcon;
  QMenu *trayIconMenu;
  QAction *playPauseAction;
  QAction *stopAction;
  QAction *prevAction;
  QAction *nextAction;
  QAction *showSongMetadataAction;
  QAction *consumeAction;
  QAction *quitAction;
  QAction *aboutAction;
  bool consumePingpong;
  bool nonConsumeSlider;
  bool show_metadata_on_mouse_leave_;
  int collapsedHeight_;
  int fullHeight_;

  static const int constBlurRadius_;

  int showMpdConnectionDialog();
  bool setupTrayIcon();
  void doConsumePingpong();
  void restoreTrackSliderHandle();
  void setTrackSliderHandleToConsume();

 private slots:
  void expandCollapse();
  void showVolumeSlider();
  void updateStats();
  void updateStatus();
  void playPauseTrack() const;
  void stopTrack() const;
  void positionSliderPressed();
  void setPosition() const;
  void seekBackward() const;
  void seekForward() const;
  void positionSliderReleased();
  void setVolume(const quint8 value) const;
  void showCurrentSongMetadata();

 signals:
  void submitSong();
  void startSong();
  void stopSong();
  void pauseSong();
  void resumeSong();
  void nowPlaying();
  void songChanged(quint32 song);
};

#endif  // PLAYER_H
