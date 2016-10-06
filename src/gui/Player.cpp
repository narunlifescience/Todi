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

#include "Player.h"
#include "IconLoader.h"
#include "ui_Player.h"

#include <QLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QSettings>
#include <QSizeGrip>
#include <QSpacerItem>
#include <QTimer>
#include <QWidget>

#include <math.h>
#include <QGraphicsDropShadowEffect>
#include <QMenu>
#include <QMouseEvent>
#include <QPalette>
#include <QStyle>
#include <QStyleOptionSlider>
#include <QTimer>
#include <QWheelEvent>
#include <memory>

#include "AboutDialog.h"
#include "MpdConnectionDialog.h"
#include "widgets/TrackSlider.h"
#include "globals.h"

#include "mpdclient.h"
#include "mpddata.h"
#include "playbackcontroller.h"

Player::Player(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                          Qt::SubWindow),
      ui_(new Ui_Player),
      mpdClient_(new MPDClient(this)),
      dataAccess_(nullptr),
      playbackCtrlr_(nullptr),
      lastState(MPDPlaybackState::Inactive),
      lastSongId(-1),
      fetchStatsFactor(0),
      nowPlayingFactor(0),
      draggingPositionSlider(false),
      widget(new QWidget(this)),
      close_pushButton(new QPushButton(this)),
      expand_collapse_PushButton(new QPushButton(this)),
      previous_pushButton(new QPushButton(this)),
      play_pause_pushButton(new QPushButton(this)),
      next_pushButton(new QPushButton(this)),
      volume_pushButton(new QPushButton(this)),
      search_pushButton(new QPushButton(this)),
      playlist_pushButton(new QPushButton(this)),
      track_slider(new TrackSlider(Qt::Horizontal, this)),
      timer_label(new QLabel(this)),
      albumcover_label(new QLabel(this)),
      songMetadata_label(new QLabel(this)),
      volume_slider_frame(new QFrame(this, Qt::Popup)),
      volume_slider(new QSlider(Qt::Horizontal, volume_slider_frame)),
      resize_status(false) {
  ui_->setupUi(this);

  IconLoader::init();
  IconLoader::lumen_ = IconLoader::isLight(Qt::black);

  dataAccess_ = mpdClient_->getSharedMPDdataPtr();
  playbackCtrlr_ = mpdClient_->getSharedPlaybackControllerPtr();

  this->resize(this->width(), 61);
  this->setAttribute(Qt::WA_TranslucentBackground, true);
  setWindowTitle(tr("Todi"));
  setWindowIcon(QIcon(":icons/todi.svg"));

  QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
  effect->setColor(QColor(181, 185, 190));
  effect->setBlurRadius(5);
  effect->setXOffset(0);
  effect->setYOffset(0);
  setGraphicsEffect(effect);
  this->setGraphicsEffect(effect);
  qInfo() << "Starting Todi application...";

  // Set icons
  close_pushButton->setIcon(
      IconLoader::load("edit-close", IconLoader::LightDark));
  expand_collapse_PushButton->setIcon(
      IconLoader::load("edit-expand", IconLoader::LightDark));
  previous_pushButton->setIcon(
      IconLoader::load("media-skip-backward", IconLoader::LightDark));
  play_pause_pushButton->setIcon(
      IconLoader::load("media-playback-start", IconLoader::LightDark));
  next_pushButton->setIcon(
      IconLoader::load("media-skip-forward", IconLoader::LightDark));
  volume_pushButton->setIcon(
      IconLoader::load("media-playback-volume", IconLoader::LightDark));
  search_pushButton->setIcon(
      IconLoader::load("edit-find", IconLoader::LightDark));
  playlist_pushButton->setIcon(
      IconLoader::load("view-media-playlist", IconLoader::LightDark));

  // Right click context menu
  QAction *quitAction = new QAction(tr("&Exit"), this);
  QAction *about = new QAction(tr("&About"), this);
  addAction(quitAction);
  addAction(about);
  setContextMenuPolicy(Qt::ActionsContextMenu);

  // Theme adjust acordingly
  widget->setStyleSheet(
      ".QWidget{border-radius: 3px; background-color: rgba(20, 20, 20, 200); "
      "border: 0px solid #5c5c5c;}");
  track_slider->setStyleSheet(
      ".TrackSlider::groove:horizontal { border: 0px solid #999999; height: "
      "3px; "
      "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, "
      "stop:1 #c4c4c4); "
      "margin: 0px 0; } .TrackSlider::handle:horizontal {"
      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eff0f1, "
      "stop:1 #eff0f1);"
      "border: 0px solid #5c5c5c;width: 2px;margin: -2px 0; }"
      ".TrackSlider::sub-page:horizontal {"
      "background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,"
      "stop: 0 #3daee9, stop: 1 #3daee9); width: 3px; margin: 0px 0;}");
  close_pushButton->setStyleSheet("QPushButton{border: none;}");
  expand_collapse_PushButton->setStyleSheet("QPushButton{border: none;}");
  previous_pushButton->setStyleSheet("QPushButton{border: none;}");
  play_pause_pushButton->setStyleSheet("QPushButton{border: none;}");
  next_pushButton->setStyleSheet("QPushButton{border: none;}");
  volume_pushButton->setStyleSheet("QPushButton{border: none;}");
  search_pushButton->setStyleSheet("QPushButton{border: none;}");
  playlist_pushButton->setStyleSheet("QPushButton{border: none;}");
  timer_label->setStyleSheet("QLabel{color:rgba(200, 200, 200, 200)}");

  QGridLayout *baselayout = new QGridLayout(this);
  baselayout->addWidget(widget);

  QVBoxLayout *expandcollapsecloselayout = new QVBoxLayout;
  expandcollapsecloselayout->setContentsMargins(0, 3, 0, 3);
  expandcollapsecloselayout->addWidget(close_pushButton);
  expandcollapsecloselayout->addStretch();
  expandcollapsecloselayout->addWidget(expand_collapse_PushButton);

  QHBoxLayout *playerbuttonslayout = new QHBoxLayout;
  playerbuttonslayout->addWidget(previous_pushButton);
  playerbuttonslayout->addWidget(play_pause_pushButton);
  playerbuttonslayout->addWidget(next_pushButton);

  QHBoxLayout *slidertimerlayout = new QHBoxLayout;
  slidertimerlayout->setContentsMargins(0, 0, 3, 3);
  slidertimerlayout->setSpacing(3);
  slidertimerlayout->addWidget(track_slider);
  slidertimerlayout->addWidget(timer_label);

  QHBoxLayout *layout = new QHBoxLayout();
  layout->setContentsMargins(0, 3, 3, 0);
  layout->addStretch();
  layout->addLayout(playerbuttonslayout);
  layout->addStretch();
  layout->addWidget(volume_pushButton);
  layout->addWidget(search_pushButton);
  layout->addWidget(playlist_pushButton);

  QVBoxLayout *vboxplaypauseslidertimer = new QVBoxLayout();
  vboxplaypauseslidertimer->addLayout(layout);
  vboxplaypauseslidertimer->addLayout(slidertimerlayout);

  QHBoxLayout *hboxfinal = new QHBoxLayout(widget);
  // hboxfinal->setContentsMargins(3, 3, 0, 3);
  hboxfinal->setContentsMargins(0, 0, 0, 0);
  hboxfinal->setSpacing(5);
  // hboxfinal->addWidget(new QSizeGrip(widget), 0, Qt::AlignBottom |
  // Qt::AlignLeft);
  hboxfinal->addLayout(expandcollapsecloselayout, 0);
  hboxfinal->addWidget(albumcover_label, 1);
  hboxfinal->addWidget(songMetadata_label, 2);
  hboxfinal->addLayout(vboxplaypauseslidertimer, 3);
  // hboxfinal->addWidget(new QSizeGrip(widget), 0, Qt::AlignBottom |
  // Qt::AlignRight);
  songMetadata_label->setVisible(false);
  songMetadata_label->setText("test Todi compact");

  widget->layout()->setSizeConstraint(QLayout::SetNoConstraint);
  timer_label->setText("--:--");
  QPixmap pixmap(":/icons/nocover.svg");
  albumcover_label->setFixedWidth(this->height() - 20);
  albumcover_label->setFixedHeight(this->height() - 20);
  albumcover_label->resize(this->height() - 20, this->height() - 20);
  albumcover_label->setPixmap(
      pixmap.scaled(albumcover_label->size(), Qt::KeepAspectRatio));
  albumcover_label->setPixmap(pixmap.scaled(albumcover_label->size(),
                                            Qt::KeepAspectRatioByExpanding,
                                            Qt::SmoothTransformation));
  albumcover_label->setScaledContents(true);

  volume_slider_frame->resize(100, 40);
  volume_slider->setMinimum(0);
  volume_slider->setMaximum(100);

  QHBoxLayout *volumeslider = new QHBoxLayout(volume_slider_frame);
  volumeslider->addWidget(volume_slider);

  this->setMouseTracking(true);

  widget->installEventFilter(widget);

  // Set connection data
  QSettings settings;
  // Tray stuf
  // if (setupTrayIcon() && settings.value("systemtray").toBool())
  trayIcon = new QSystemTrayIcon(this);
  trayIcon->setIcon(QIcon(":icons/todi.svg"));
  trayIcon->setToolTip("Todi");
  trayIcon->show();
  settings.beginGroup("mpd-server-connection");
  Todi::hostname = settings.value("host", "localhost").toString();
  Todi::port = static_cast<quint16>(settings.value("port", 6600).toUInt());
  settings.endGroup();

  mpdClient_->connectToHost(Todi::hostname, Todi::port, "");

  /*while (!mpd.connectToMPD()) {
    qWarning() << "Retrying to connect...";
    if (!showMpdConnectionDialog()) {
      qCritical()
          << "MPD connection couldnot be established! Exiting application...";
      exit(EXIT_FAILURE);
    }
    mpd.setHostname(Todi::hostname);
    mpd.setPort(Todi::port);
    mpdDb.setHostname(Todi::hostname);
    mpdDb.setPort(Todi::port);
    qWarning() << "Unable to connect to MPD with Hostname : " << Todi::hostname
               << " Port : " << Todi::port;
  }*/

  // MPD
  connect(dataAccess_.get(), &MPDdata::MPDStatsUpdated, this,
          &Player::updateStats);
  connect(dataAccess_.get(), &MPDdata::MPDStatusUpdated, this,
          &Player::updateStatus, Qt::DirectConnection);

  auto quitApplication = []() { QApplication::quit(); };
  connect(quitAction, &QAction::triggered, quitApplication);
  connect(close_pushButton, &QPushButton::clicked,
          [&]() { (trayIcon->isVisible()) ? hide() : QApplication::quit(); });
  connect(about, &QAction::triggered, []() {
    std::unique_ptr<AboutDialog> abt(new AboutDialog());
    abt->exec();
  });
  connect(trayIcon, &QSystemTrayIcon::activated, this,
          [&]() { (isHidden()) ? show() : hide(); });
  connect(expand_collapse_PushButton, &QPushButton::clicked, this,
          &Player::expandCollapse);

  // Basic media buttons
  connect(play_pause_pushButton, &QPushButton::clicked, this,
          &Player::playPauseTrack);
  connect(previous_pushButton, &QPushButton::clicked, [&]() {
    playbackCtrlr_->previous();
    dataAccess_->getMPDStatus();
  });
  connect(next_pushButton, &QPushButton::clicked, [&]() {
    playbackCtrlr_->next();
    dataAccess_->getMPDStatus();
  });

  // volume & track slider
  connect(track_slider, &TrackSlider::sliderPressed, this,
          &Player::positionSliderPressed);
  connect(track_slider, &TrackSlider::sliderReleased, this,
          &Player::setPosition);
  connect(track_slider, &TrackSlider::sliderReleased, this,
          &Player::positionSliderReleased);
  connect(track_slider, SIGNAL(seekBackward()), this, SLOT(seekBackward()));
  connect(track_slider, SIGNAL(seekForward()), this, SLOT(seekForward()));
  // connect(track_slider, &TrackSlider::seekBackward, this,
  //        &Player::seekBackward);
  // connect(volume_slider, &QSlider::valueChanged,
  //        [&](int vol) { mpd.setVolume(static_cast<quint8>(vol)); });
  connect(volume_pushButton, &QPushButton::clicked, this,
          &Player::showVolumeSlider);

  // Timer time out update status
  statusTimer.start(settings.value("getstatus-interval", 1000).toInt());
  //statusTimer.start(10000);
  connect(&statusTimer, &QTimer::timeout, dataAccess_.get(),
          &MPDdata::getMPDStatus);

  // update status & stats when starting the application
  dataAccess_->getMPDStatus();
  dataAccess_->getMPDStats();
}

QSize Player::sizeHint() const { return QSize(width(), 61); }

Player::~Player() { delete ui_; }

void Player::mousePressEvent(QMouseEvent *event) {
  dragPosition = event->pos();
  QWidget::mousePressEvent(event);
  if (event->button() == Qt::LeftButton) {
    setCursor(Qt::SizeAllCursor);
  }
}

void Player::mouseReleaseEvent(QMouseEvent *event) {
  dragPosition = QPoint();
  QWidget::mouseReleaseEvent(event);
  if (event->button() == Qt::LeftButton) {
    setCursor(Qt::ArrowCursor);
  }
}

void Player::leaveEvent(QEvent *) {
  /*songMetadata_label->show();
  previous_pushButton->hide();
  play_pause_pushButton->hide();
  next_pushButton->hide();
  volume_pushButton->hide();
  track_slider->hide();
  update();*/
}

void Player::enterEvent(QEvent *) {}

void Player::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() & Qt::LeftButton) {
    if (resize_status) {
      QPoint p = mapToGlobal(event->pos()) - geometry().topLeft();
      resize(p.x(), p.y());
    } else {
      move(mapToParent(event->pos() - dragPosition));
    }
  } else {
    QPoint diff = geometry().bottomRight() - mapToGlobal(event->pos());
    if (diff.x() <= 10 && diff.y() <= 10) {
      if (!resize_status) {
        setCursor(Qt::SizeFDiagCursor);
        resize_status = true;
      }
    } else {
      if (resize_status) {
        setCursor(Qt::ArrowCursor);
        resize_status = false;
      }
    }
  }
}

void Player::resizeEvent(QResizeEvent *) {
  if (widget->width() > 200) {
    albumcover_label->show();
  } else {
    albumcover_label->hide();
  }
  if (widget->width() > 350) {
    search_pushButton->show();
    playlist_pushButton->show();
  } else {
    search_pushButton->hide();
    playlist_pushButton->hide();
  }
}

int Player::showMpdConnectionDialog() {
  std::unique_ptr<MpdConnectionDialog> connect(new MpdConnectionDialog(this));
  return connect->exec();
}

void Player::expandCollapse() {
  /*if (height() == 61) {
    expand_collapse_PushButton->setIcon(
        IconLoader::load("edit-collapse", IconLoader::LightDark));
    resize(width(), width());
  } else {
      setFixedHeight(61);
    expand_collapse_PushButton->setIcon(
        IconLoader::load("edit-expand", IconLoader::LightDark));
    //this->resize(this->width(), 61);
  }*/
}

void Player::showVolumeSlider() {
  // volume_slider_frame->setFrameStyle(3);
  // volume_slider_frame->setFrameShape(QFrame::StyledPanel);
  volume_slider_frame->show();
  // volume_slider_frame->move(volume_pushButton->x(), volume_pushButton->y());
  volume_slider_frame->move(
      mapToGlobal(volume_pushButton->geometry().topLeft()));
}

void Player::updateStats() {
  //MPDStats *const stats = MPDStats::getInstance();

  /*
   * Check if remote db is more recent than local one
   * Also update the dirview
   */
  /*if (lastDbUpdate.isValid() && stats->dbUpdate() > lastDbUpdate) {
    mpdDb.listAllInfo(stats->dbUpdate());
    mpdDb.listAll();
  }

  lastDbUpdate = stats->dbUpdate();*/
}

void Player::updateStatus() {
  QString timeElapsedFormattedString;

  // Retrieve stats every 5 seconds
  // fetchStatsFactor = (fetchStatsFactor + 1) % 5;
  // if (fetchStatsFactor == 0) mpd.getStats();

  if (!draggingPositionSlider) {
    if (dataAccess_->state() == MPDPlaybackState::Stopped ||
        dataAccess_->state() == MPDPlaybackState::Inactive) {
      track_slider->setValue(0);
    } else {
      track_slider->setRange(0, dataAccess_->timeTotal());
      track_slider->setValue(dataAccess_->timeElapsed());
    }
  }

  /*volume_slider->setValue(dataAccess_->volume());
  if (dataAccess_->timeElapsed() != 0)
    setIconProgress(dataAccess_->timeElapsed() * 100 /
                    dataAccess_->timeTotal());
*/
  if (dataAccess_->state() == MPDPlaybackState::Stopped ||
      dataAccess_->state() == MPDPlaybackState::Inactive) {
    timeElapsedFormattedString = "00:00";
  } else {
    timeElapsedFormattedString +=
        QString::number(floor(dataAccess_->timeElapsed() / 60.0));
    timeElapsedFormattedString += ":";
    if (dataAccess_->timeElapsed() % 60 < 10) timeElapsedFormattedString += "0";
    timeElapsedFormattedString +=
        QString::number(dataAccess_->timeElapsed() % 60);
  }

  timer_label->setText(timeElapsedFormattedString);

  switch (dataAccess_->state()) {
    case MPDPlaybackState::Playing:
      // Main window
      play_pause_pushButton->setIcon(
          IconLoader::load("media-playback-pause", IconLoader::LightDark));
      play_pause_pushButton->setEnabled(true);
      break;

    case MPDPlaybackState::Inactive:
    case MPDPlaybackState::Stopped:
      // Main window
      play_pause_pushButton->setIcon(
          IconLoader::load("media-playback-start", IconLoader::LightDark));
      play_pause_pushButton->setEnabled(true);
      setIconProgress(100);
      break;

    case MPDPlaybackState::Paused:
      // Main window
      play_pause_pushButton->setIcon(
          IconLoader::load("media-playback-start", IconLoader::LightDark));
      play_pause_pushButton->setEnabled(true);
      break;
      qDebug("Invalid state");
  }

  // Check if song has changed or we're playing again after being stopped
  // and update song info if needed
  if (lastState == MPDPlaybackState::Inactive ||
      (lastState == MPDPlaybackState::Stopped &&
       dataAccess_->state() == MPDPlaybackState::Playing) ||
      lastSongId != dataAccess_->songId()) {
  }
  // mpd.currentSong();

  /* Update libre scrobbler stuff (no librefm support for now)*/
  /*if (settings.value("lastfm/enabled", "false").toBool()) {
    if (lastState == MPDStatus::State_Playing &&
        status->state() == MPDStatus::State_Paused) {
      emit pauseSong();
    } else if (lastState == MPDStatus::State_Paused &&
               status->state() == MPDStatus::State_Playing) {
      emit resumeSong();
      emit nowPlaying();
    } else if (lastState != MPDStatus::State_Stopped &&
               status->state() == MPDStatus::State_Stopped) {
      emit stopSong();
    }

    if (status->state() == MPDStatus::State_Playing) {
      nowPlayingFactor = (nowPlayingFactor + 1) % 20;
      if (nowPlayingFactor == 0) emit nowPlaying();
    }
  }*/

  // Set TrayIcon tooltip
  QString text;
  text += "time: " + timeElapsedFormattedString;
  if (trayIcon != NULL) trayIcon->setToolTip(text);

  // Check if playlist has changed and update if needed
  // if (lastState == MPDStatus::State::StateInactive ||
  //    lastPlaylist < status->playlist()) {
  //  mpd.playListInfo();
  //}

  // Display bitrate
  bitrateLabel.setText("Bitrate: " + QString::number(dataAccess_->bitrate()));

  // Update status info
  lastState = dataAccess_->state();
  lastSongId = dataAccess_->songId();
  // lastPlaylist = dataAccess_->playlist();
}

void Player::playPauseTrack() {
  if (dataAccess_->state() == MPDPlaybackState::Playing)
    playbackCtrlr_->pause(1);
  else if (dataAccess_->state() == MPDPlaybackState::Paused)
    playbackCtrlr_->pause(0);
  else
    playbackCtrlr_->playId(lastSongId);

  dataAccess_->getMPDStatus();
}

void Player::stopTrack() {
  playbackCtrlr_->stop();
  dataAccess_->getMPDStatus();
}

void Player::positionSliderPressed() { draggingPositionSlider = true; }

void Player::setPosition() {
  playbackCtrlr_->seekId(static_cast<quint32>(dataAccess_->songId()),
                         static_cast<quint32>(track_slider->value()));
  dataAccess_->getMPDStatus();
}

void Player::seekBackward() {
  playbackCtrlr_->seekId(static_cast<quint32>(dataAccess_->songId()),
                         static_cast<quint32>(track_slider->value() - 10));
  dataAccess_->getMPDStatus();
}

void Player::seekForward() {
  playbackCtrlr_->seekId(static_cast<quint32>(dataAccess_->songId()),
                         static_cast<quint32>(track_slider->value() + 10));
  dataAccess_->getMPDStatus();
}

void Player::positionSliderReleased() { draggingPositionSlider = false; }

void Player::setAlbumCover(QImage image, QString artist, QString album) {
  Q_UNUSED(artist);
  Q_UNUSED(album);

  qDebug("setalbum cover ()");
  if (image.isNull()) {
    albumcover_label->setText("No cover available");
    return;
  }

  // Display image
  QPixmap pixmap = QPixmap::fromImage(image);
  pixmap = pixmap.scaled(QSize(150, 150), Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
  albumcover_label->setPixmap(pixmap);

  // Save image to avoid downloading it next time
  /*QDir dir(QDir::home());
  if (!dir.exists(".Todi")) {
    if (!dir.mkdir(".Todi")) {
      qWarning("Couldn't create directory for storing album covers");
      return;
    }
  }

  dir.cd(".Todi");
  QString file(QFile::encodeName(artist + " - " + album + ".jpg"));
  image.save(dir.absolutePath() + QDir::separator() + file);*/
}
