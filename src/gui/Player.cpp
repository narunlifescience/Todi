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

#include <QBitmap>
#include <QBuffer>
#include <QLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QSettings>
#include <QSizeGrip>
#include <QSpacerItem>
#include <QTimer>
#include <QWidget>

#include <math.h>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPalette>
#include <QPixmapCache>
#include <QStyle>
#include <QStyleOptionSlider>
#include <QTimer>
#include <QToolTip>
#include <QWheelEvent>

#include "AboutDialog.h"
#include "MpdConnectionDialog.h"
#include "globals.h"
#include "widgets/TrackSlider.h"
#include "widgets/VolumePopup.h"
#include "widgets/songmetadatalabel.h"

#include "mpdclient.h"
#include "mpddata.h"
#include "playbackcontroller.h"
#include "playbackoptionscontroller.h"
#include "tagger/tagreader.h"

const int Player::constBlurRadius_ = 5;

Player::Player(QWidget *parent)
    : QWidget(
          parent,
          Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow),
      ui_(new Ui_Player),
      mpdClient_(new MPDClient(this)),
      dataAccess_(nullptr),
      playbackCtrlr_(nullptr),
      playbackOptionsCtrlr_(nullptr),
      tagreader_(new TagReader(this)),
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
      songMetadata_label(new SongMetadataLabel(this)),
      volume_popup(new VolumePopup(this)),
      resize_status(false),
      consumePingpong(true),
      nonConsumeSlider(true),
      systemTrayProgress(SystemTrayProgress::EighthOctave) {
  ui_->setupUi(this);

  IconLoader::init();
  IconLoader::lumen_ = IconLoader::isLight(Qt::black);

  dataAccess_ = mpdClient_->getSharedMPDdataPtr();
  playbackCtrlr_ = mpdClient_->getSharedPlaybackControllerPtr();
  playbackOptionsCtrlr_ = mpdClient_->getSharedPlaybackOptionsControllerPtr();

  this->setFixedHeight(50);
  widget->setFixedHeight(40);
  this->setAttribute(Qt::WA_TranslucentBackground, true);
  setWindowTitle(QApplication::applicationName());
  setWindowIcon(QIcon(":icons/todi.svg"));

  QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
  effect->setColor(QColor(181, 185, 190));
  effect->setBlurRadius(constBlurRadius_);
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

  // initialize QActions
  showSongMetadataAction = new QAction(tr("&Show Song Metadata"), this);
  consumeAction = new QAction(tr("&Consume"), this);
  consumeAction->setCheckable(true);
  quitAction = new QAction(tr("&Exit"), this);
  aboutAction = new QAction(tr("&About"), this);

  // Right click context menu
  addAction(showSongMetadataAction);
  addAction(consumeAction);
  addAction(quitAction);
  addAction(aboutAction);
  setContextMenuPolicy(Qt::ActionsContextMenu);

  quitAction->setIcon(IconLoader::load("edit-close", IconLoader::LightDark));

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
  baselayout->setContentsMargins(5, 5, 5, 5);
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

  //previous_pushButton->hide();
  //play_pause_pushButton->hide();
  //next_pushButton->hide();
  //volume_pushButton->hide();
  //track_slider->hide();
  //timer_label->hide();

  QHBoxLayout *hboxfinal = new QHBoxLayout(widget);
  // hboxfinal->setContentsMargins(3, 3, 0, 3);
  hboxfinal->setContentsMargins(0, 0, 0, 0);
  hboxfinal->setSpacing(5);
  // hboxfinal->addWidget(new QSizeGrip(widget), 0, Qt::AlignBottom |
  // Qt::AlignLeft);
  songMetadata_label->hide();
  hboxfinal->addLayout(expandcollapsecloselayout, 0);
  hboxfinal->addWidget(albumcover_label, 1);
  hboxfinal->addWidget(songMetadata_label, 2);
  hboxfinal->addLayout(vboxplaypauseslidertimer, 3);
  // hboxfinal->addWidget(new QSizeGrip(widget), 0, Qt::AlignBottom |
  // Qt::AlignRight);
  songMetadata_label->setText("Todi");

  widget->layout()->setSizeConstraint(QLayout::SetNoConstraint);
  timer_label->setText("--:--");
  QPixmap pixmap(":/icons/nocover.svg");
  albumcover_label->setFixedWidth(widget->height());
  albumcover_label->setFixedHeight(widget->height());
  albumcover_label->resize(widget->height(), widget->height());
  albumcover_label->setPixmap(
      pixmap.scaled(albumcover_label->size(), Qt::KeepAspectRatio));
  albumcover_label->setPixmap(pixmap.scaled(albumcover_label->size(),
                                            Qt::KeepAspectRatioByExpanding,
                                            Qt::SmoothTransformation));
  albumcover_label->setScaledContents(true);

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

  while (!mpdClient_->connectToHost(Todi::hostname, Todi::port, "")) {
    qWarning() << "Retrying to connect...";
    if (!showMpdConnectionDialog()) {
      qCritical()
          << "MPD connection couldnot be established! Exiting application...";
      exit(EXIT_FAILURE);
    }
    qWarning() << "Unable to connect to MPD with Hostname : " << Todi::hostname
               << " Port : " << Todi::port;
  }

  // restore window geometry
  settings.beginGroup("player");
  restoreGeometry(settings.value("geometry").toByteArray());
  settings.endGroup();

  // MPD
  connect(dataAccess_.get(), &MPDdata::MPDStatsUpdated, this,
          &Player::updateStats);
  connect(dataAccess_.get(), &MPDdata::MPDStatusUpdated, this,
          &Player::updateStatus, Qt::DirectConnection);

  connect(showSongMetadataAction, &QAction::triggered, this,
          &Player::showCurrentSongMetadata);
  auto quitApplication = [&]() {
    QSettings settings;
    settings.beginGroup("player");
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();
    QApplication::quit();
  };
  connect(quitAction, &QAction::triggered, quitApplication);
  connect(close_pushButton, &QPushButton::clicked,
          [&]() { (trayIcon->isVisible()) ? hide() : QApplication::quit(); });
  connect(aboutAction, &QAction::triggered, []() {
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
  connect(track_slider, &TrackSlider::valueChanged, this,
          &Player::trayIconUpdateProgress);
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
  // statusTimer.start(10000);
  connect(&statusTimer, &QTimer::timeout, dataAccess_.get(),
          &MPDdata::getMPDStatus);

  // Volume popup signal handling
  connect(volume_popup, &VolumePopup::volumePopupSliderChanged, this,
          &Player::setVolume);

  // Cover art loading
  connect(tagreader_, &TagReader::coverArtProcessed, this,
          &Player::setAlbumCover);
  connect(dataAccess_.get(), &MPDdata::MPDSongMetadataUpdated, tagreader_,
          &TagReader::loadCoverArt);

  // update status & stats when starting the application
  dataAccess_->getMPDStatus();
  dataAccess_->getMPDStats();
}

QSize Player::sizeHint() const { return QSize(width(), 50); }

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
  previous_pushButton->hide();
  play_pause_pushButton->hide();
  next_pushButton->hide();
  volume_pushButton->hide();
  track_slider->hide();
  timer_label->hide();
  songMetadata_label->show();
}

void Player::enterEvent(QEvent *) {
  songMetadata_label->hide();
  previous_pushButton->show();
  play_pause_pushButton->show();
  next_pushButton->show();
  volume_pushButton->show();
  track_slider->show();
  timer_label->show();
}

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

  songMetadata_label->setFixedWidth(track_slider->width() +
                                    timer_label->width());
  // songMetadata_label->setFixedWidth(layout->contentsRect().width() - 10);
  // while resizing we dont want animation (so pass false as arg)
  songMetadata_label->updateSongMetadataText(false);
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
  volume_popup->show();
  QPoint position = volume_pushButton->pos();

  position.setX(position.x() + (volume_pushButton->width() / 2));
  position.setX(position.x() - (volume_popup->width() / 2));
  position.setY(position.y() + (volume_pushButton->height() / 2));
  position.setY(position.y() -
                (volume_popup->height() - (VolumePopup::blur_padding * 2)));
  volume_popup->move(mapToGlobal(position));
}

void Player::updateStats() {
  // MPDStats *const stats = MPDStats::getInstance();

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

  if (dataAccess_->consume()) {
    if (dataAccess_->state() == MPDPlaybackState::Playing) {
      doConsumePingpong();
    } else {
      setTrackSliderHandleToConsume();
    }
  } else {
    if (!nonConsumeSlider) {
      restoreTrackSliderHandle();
    }
  }

  volume_popup->setVolumeSlider(dataAccess_->volume());
  /* if (dataAccess_->timeElapsed() != 0)
     setIconProgress(dataAccess_->timeElapsed() * 100 /
                     dataAccess_->timeTotal());
 */
  if (dataAccess_->state() == MPDPlaybackState::Stopped ||
      dataAccess_->state() == MPDPlaybackState::Inactive) {
    play_pause_pushButton->setIcon(
        IconLoader::load("media-playback-start", IconLoader::LightDark));
    play_pause_pushButton->setEnabled(true);
    timer_label->setText("00:00");
    return;
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
    dataAccess_->getMPDSongMetadata();
    // truncateSongMetadataLabelString();
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
  text += "Todi: " + timeElapsedFormattedString;
  if (trayIcon != nullptr) trayIcon->setToolTip(text);

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
    // playbackCtrlr_->playId(lastSongId);
    //
    playbackCtrlr_->play(0);

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

void Player::setAlbumCover(QImage image) {
  qDebug("setalbum cover ()");
  if (image.isNull()) {
    albumcover_label->setText("No cover available");
    return;
  }

  // Display image
  albumcover_label->setScaledContents(true);
  QPixmap originalPix = QPixmap::fromImage(image);
  QPixmap pixmap = originalPix;
  pixmap = pixmap.scaled(albumcover_label->size(), Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
  QPainter painter(&pixmap);
  QPen pen(QColor(242, 242, 242), 2);
  painter.setPen(pen);
  painter.drawRect(pixmap.rect());

  albumcover_label->setPixmap(pixmap);

  // Tooltip
  QString toolTip = QString("<table>");
  toolTip += QString(
                 "<tr><td align=\"right\"><b>Artist:</b></td><td>%1</td></tr>"
                 "<tr><td align=\"right\"><b>Album:</b></td><td>%2</td></tr>"
                 "<tr><td align=\"right\"><b>Year:</b></td><td>%3</td></tr>")
                 .arg(dataAccess_->artist())
                 .arg(dataAccess_->album())
                 .arg(QString::number(dataAccess_->date()));
  toolTip += "</table>";
  QByteArray bytes;
  QBuffer buffer(&bytes);
  originalPix.save(&buffer, "PNG", 100);
  toolTip += QString("<br/><img src=\"data:image/png;base64, %0\"/>")
                 .arg(QString(bytes.toBase64()));
  albumcover_label->setToolTip(toolTip);
  songMetadata_label->updateSongMetadata(dataAccess_->title(),
                                         dataAccess_->album());

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

void Player::trayIconUpdateProgress(int value) {
  if (value != 0) {
    SystemTrayProgress trayProgress = SystemTrayProgress::EighthOctave;
    int percent = (value * 100) / track_slider->maximum();

    (percent < 12)
        ? trayProgress = SystemTrayProgress::FirstOctave
        : (percent < 24)
              ? trayProgress = SystemTrayProgress::SecondOctave
              : (percent < 36)
                    ? trayProgress = SystemTrayProgress::ThirdOctave
                    : (percent < 48)
                          ? trayProgress = SystemTrayProgress::FourthOctave
                          : (percent < 60)
                                ? trayProgress = SystemTrayProgress::FifthOctave
                                : (percent < 72)
                                      ? trayProgress =
                                            SystemTrayProgress::SixthOctave
                                      : (percent < 84)
                                            ? trayProgress =
                                                  SystemTrayProgress::
                                                      SeventhOctave
                                            : (percent < 96)
                                                  ? trayProgress =
                                                        SystemTrayProgress::
                                                            EighthOctave
                                                  : trayProgress =
                                                        SystemTrayProgress::
                                                            EighthOctave;

    if (trayProgress != systemTrayProgress) {
      setTrayIconProgress(trayProgress);
    }

  } else {
    if (systemTrayProgress != SystemTrayProgress::EighthOctave) {
      setTrayIconProgress(SystemTrayProgress::EighthOctave);
    }
  }
}

void Player::setVolume(quint8 value) { playbackOptionsCtrlr_->setvol(value); }

void Player::showCurrentSongMetadata() {
  QMessageBox mBox;
  mBox.setWindowTitle("Song Metadata");
  // dataAccess_->getMPDSongMetadata();
  QString metadata;
  metadata = QString("File : ") + dataAccess_->file() + "\n";
  metadata = metadata + QString("Artist : ") + dataAccess_->artist() + "\n";
  metadata = metadata + QString("Album : ") + dataAccess_->album() + "\n";
  metadata = metadata + QString("AlbumId : ") + dataAccess_->albumId() + "\n";
  metadata =
      metadata + QString("Album Artist : ") + dataAccess_->albumArtist() + "\n";
  metadata = metadata + QString("Title : ") + dataAccess_->title() + "\n";
  metadata = metadata + QString("Track : ") +
             QString::number(dataAccess_->track()) + "\n";
  metadata = metadata + QString("Name : ") + dataAccess_->name() + "\n";
  metadata = metadata + QString("Genre : ") + dataAccess_->genre() + "\n";
  metadata = metadata + QString("Date : ") +
             QString::number(dataAccess_->date()) + "\n";
  metadata = metadata + QString("Composer : ") + dataAccess_->comment() + "\n";
  metadata =
      metadata + QString("Performer : ") + dataAccess_->performer() + "\n";
  metadata = metadata + QString("Comment : ") + dataAccess_->comment() + "\n";
  metadata = metadata + QString("Disc : ") +
             QString::number(dataAccess_->disc()) + "\n";
  metadata = metadata + QString("Time : ") +
             QString::number(dataAccess_->time()) + "\n";
  metadata =
      metadata + QString("Id : ") + QString::number(dataAccess_->id()) + "\n";
  metadata = metadata + QString("Last Modified : ") +
             dataAccess_->lastModified() + "\n";
  metadata =
      metadata + QString("Pos : ") + QString::number(dataAccess_->pos()) + "\n";

  mBox.setText(metadata);
  mBox.exec();
}

void Player::setTrayIconProgress(Player::SystemTrayProgress trayProgress) {
  QPixmap traypix;
  switch (trayProgress) {
    case SystemTrayProgress::FirstOctave:

      if (!QPixmapCache::find("onebyeight", &traypix)) {
        traypix.load(":/icons/tray/onebyeight.svg");
        QPixmapCache::insert("onebyeight", traypix);
      }
      trayIcon->setIcon(traypix);
      break;
    case SystemTrayProgress::SecondOctave:
      if (!QPixmapCache::find("twobyeight", &traypix)) {
        traypix.load(":/icons/tray/twobyeight.svg");
        QPixmapCache::insert("twobyeight", traypix);
      }
      trayIcon->setIcon(traypix);
      break;
    case SystemTrayProgress::ThirdOctave:
      if (!QPixmapCache::find("threebyeight", &traypix)) {
        traypix.load(":/icons/tray/threebyeight.svg");
        QPixmapCache::insert("threebyeight", traypix);
      }
      trayIcon->setIcon(traypix);
      break;
    case SystemTrayProgress::FourthOctave:
      if (!QPixmapCache::find("fourbyeight", &traypix)) {
        traypix.load(":/icons/tray/fourbyeight.svg");
        QPixmapCache::insert("fourbyeight", traypix);
      }
      trayIcon->setIcon(traypix);
      break;
    case SystemTrayProgress::FifthOctave:
      if (!QPixmapCache::find("fivebyeight", &traypix)) {
        traypix.load(":/icons/tray/fivebyeight.svg");
        QPixmapCache::insert("fivebyeight", traypix);
      }
      trayIcon->setIcon(traypix);
      break;
    case SystemTrayProgress::SixthOctave:
      if (!QPixmapCache::find("sixbyeight", &traypix)) {
        traypix.load(":/icons/tray/sixbyeight.svg");
        QPixmapCache::insert("sixbyeight", traypix);
      }
      trayIcon->setIcon(traypix);
      break;
    case SystemTrayProgress::SeventhOctave:
      if (!QPixmapCache::find("sevenbyeight", &traypix)) {
        traypix.load(":/icons/tray/sevenbyeight.svg");
        QPixmapCache::insert("sevenbyeight", traypix);
      }
      trayIcon->setIcon(traypix);
      break;
    case SystemTrayProgress::EighthOctave:
      if (!QPixmapCache::find("eightbyeight", &traypix)) {
        traypix.load(":/icons/tray/eightbyeight.svg");
        QPixmapCache::insert("eightbyeight", traypix);
      }
      trayIcon->setIcon(traypix);
      break;
  }
  systemTrayProgress = trayProgress;
}

void Player::doConsumePingpong() {
  if (consumePingpong) {
    track_slider->setStyleSheet(
        ".TrackSlider::handle:horizontal{image: "
        "url(:/icons/dark/edit-consume1.svg);}");
    consumePingpong = false;
  } else {
    track_slider->setStyleSheet(
        ".TrackSlider::handle:horizontal{image: "
        "url(:/icons/dark/edit-consume2.svg);}");
    consumePingpong = true;
  }
  if (nonConsumeSlider) {
    nonConsumeSlider = false;
  }
}

void Player::restoreTrackSliderHandle() {
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
  nonConsumeSlider = true;
}

void Player::setTrackSliderHandleToConsume() {
  track_slider->setStyleSheet(
      ".TrackSlider::handle:horizontal{image: "
      "url(:/icons/dark/edit-consume2.svg); border: 0px solid #5c5c5c;width: "
      "2px;margin: -2px 0; }");
  if (nonConsumeSlider) {
    nonConsumeSlider = false;
  }
}

bool Player::setupTrayIcon() {
  if (!QSystemTrayIcon::isSystemTrayAvailable()) {
    trayIcon = nullptr;
    return false;
  }

  trayIcon = new QSystemTrayIcon(this);
  // trayIcon->installEventFilter(volumeSliderEventHandler);
  trayIconMenu = new QMenu(this);

  // Setup Actions
  playPauseAction = new QAction(tr("&Play"), trayIconMenu);
  playPauseAction->setIcon(
      IconLoader::load("media-playback-play", IconLoader::LightDark));
  connect(playPauseAction, SIGNAL(triggered()), this, SLOT(playPauseTrack()));

  stopAction = new QAction(tr("&Stop"), trayIconMenu);
  stopAction->setIcon(
      IconLoader::load("media-playback-stop", IconLoader::LightDark));
  connect(stopAction, SIGNAL(triggered()), this, SLOT(stopTrack()));

  prevAction = new QAction(tr("P&rev"), trayIconMenu);
  prevAction->setIcon(
      IconLoader::load("media-skip-backward", IconLoader::LightDark));
  connect(prevAction, SIGNAL(triggered()), this, SLOT(previousTrack()));

  nextAction = new QAction(tr("&Next"), trayIconMenu);
  nextAction->setIcon(
      IconLoader::load("media-skip-forward", IconLoader::LightDark));
  connect(nextAction, SIGNAL(triggered()), this, SLOT(nextTrack()));

  quitAction = new QAction(tr("&Quit"), trayIconMenu);
  connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
          SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));

  // Setup Menu
  trayIconMenu->addAction(prevAction);
  trayIconMenu->addAction(nextAction);
  trayIconMenu->addAction(stopAction);
  trayIconMenu->addAction(playPauseAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(quitAction);

  // trayIcon->setContextMenu(trayIconMenu);
  trayIcon->setIcon(QIcon(":icons/todi.svg"));
  trayIcon->setToolTip("Todi");

  return true;
}
