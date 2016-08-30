#ifndef PLAYER_H
#define PLAYER_H

#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QWidget>

#include "MprisHandler.h"

class Ui_Player;

class Player : public QWidget {
  Q_OBJECT

 public:
  Player(QWidget *parent = 0);
  ~Player();

 protected:
  QSize sizeHint() const Q_DECL_OVERRIDE;
  void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void leaveEvent(QEvent *) Q_DECL_OVERRIDE;
  void enterEvent(QEvent *) Q_DECL_OVERRIDE;
  void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

 private:
  Ui_Player *ui_;
  QPoint dragPosition;

  enum IconName {
    Close,
    Expand,
    Collapse,
    Previous,
    Play,
    Pause,
    Next,
    Volume,
    Searchs,
    Playlists
  };

  QWidget *widget;
  QPushButton *close_pushButton;
  QPushButton *expand_collapse_PushButton;
  QPushButton *previous_pushButton;
  QPushButton *play_pause_pushButton;
  QPushButton *next_pushButton;
  QPushButton *volume_pushButton;
  QPushButton *search_pushButton;
  QPushButton *playlist_pushButton;
  QSlider *track_slider;
  QLabel *timer_label;
  QLabel *albumcover_label;
  QLabel *songMetadata_label;
  QFrame *volume_slider_frame;
  QSlider *volume_slider;
  bool resize_status;
  MprisHandler *mprishandler_;

 private slots:
  void expandCollapse();
  void showVolumeSlider();
  void quitApplication();
  void playPause();
  void next();
  void previous();

 private:
  void setPlayPauseIcon();
};

#endif  // PLAYER_H
