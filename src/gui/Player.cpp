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
#include <QListView>
#include <QMouseEvent>
#include <QPainter>
#include <QSettings>
#include <QSizeGrip>
#include <QSpacerItem>
#include <QTimer>
#include <QTreeView>
#include <QWidget>

#include <math.h>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPalette>
#include <QPixmapCache>
#include <QScrollBar>
#include <QStackedWidget>
#include <QStyle>
#include <QStyleOptionSlider>
#include <QTableWidget>
#include <QTimer>
#include <QToolTip>
#include <QWheelEvent>

#include "../core/application.h"
#include "AboutDialog.h"
#include "MpdConnectionDialog.h"
#include "beautify/theme.h"
#include "globals.h"
#include "systemtrayicon.h"
#include "widgets/TrackSlider.h"
#include "widgets/VolumePopup.h"
#include "widgets/console/consolewidget.h"
#include "widgets/currentcoverartlabel.h"
#include "widgets/currentsongmetadatalabel.h"
#include "widgets/metadatawidget.h"
#include "widgets/tabbar.h"

#include "currentplaylistmodel.h"
#include "currentplaylistview.h"
#include "models/filemodel.h"
#include "models/librarymodel.h"
#include "mpdclient.h"
#include "mpddata.h"
#include "playbackcontroller.h"
#include "playbackoptionscontroller.h"
#include "tagger/currentartloader.h"

const int Player::constBlurRadius_ = 5;

Player::Player(Application *app, QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                          Qt::SubWindow),
      ui_(new Ui_Player),
      app_(app),
      theme_(new Theme(this)),
      mpdClient_(app_->mpdClient()),
      dataAccess_(app_->mpdClient()->getSharedMPDdataPtr()),
      playbackCtrlr_(app_->mpdClient()->getSharedPlaybackControllerPtr()),
      playbackOptionsCtrlr_(
          app_->mpdClient()->getSharedPlaybackOptionsControllerPtr()),
      currentArtLoader_(app_->currentArtLoader()),
      lastState(MPDPlaybackState::Inactive),
      lastSongId(-1),
      lastPlaylist(0),
      fetchStatsFactor(0),
      nowPlayingFactor(0),
      draggingPositionSlider(false),
      mainWidget(new QWidget(this)),
      close_pushButton(new QPushButton(mainWidget)),
      expand_collapse_PushButton(new QPushButton(mainWidget)),
      previous_pushButton(new QPushButton(mainWidget)),
      play_pause_pushButton(new QPushButton(mainWidget)),
      next_pushButton(new QPushButton(mainWidget)),
      volume_pushButton(new QPushButton(mainWidget)),
      search_pushButton(new QPushButton(mainWidget)),
      playlist_pushButton(new QPushButton(mainWidget)),
      track_slider(new TrackSlider(Qt::Horizontal, mainWidget)),
      timer_label(new QLabel(mainWidget)),
      currentCoverArt_label(new CurrentCoverArtLabel(app_, mainWidget)),
      currentSongMetadata_label(new CurrentSongMetadataLabel(app_, mainWidget)),
      volume_popup(new VolumePopup(mainWidget)),
      stack_widget(new QStackedWidget(this)),
      metadata_widget(new MetadataWidget(this)),
      fancy_tab_widget(
          new FancyTabWidget(this, FancyTabWidget::Mode::Mode_LargeSidebar)),
      console_widget_(new ConsoleWidget()),
      playlist_view(new QListView(this)),
      folder_view_(new QTreeView(this)),
      library_view_(new QTreeView(this)),
      resize_status(false),
      trayIcon(nullptr),
      consumePingpong(true),
      nonConsumeSlider(true),
      show_metadata_on_mouse_leave_(false),
      collapsedHeight_(0),
      fullHeight_(0) {
  ui_->setupUi(this);
  this->setAttribute(Qt::WA_TranslucentBackground, true);
  setWindowTitle(QApplication::applicationName());
  setWindowIcon(QIcon(":icons/todi.svg"));

  // initialize graphics effects here before defining Theme connections
  QGraphicsDropShadowEffect *playerShadowEffect =
      new QGraphicsDropShadowEffect(this);
  QGraphicsDropShadowEffect *volumepopupShadowEffect =
      new QGraphicsDropShadowEffect(this);

  // Setting Stylesheets & glow effects (make connections first & then
  // initialize default theme before doing anything)
  connect(theme_, &Theme::themePlayerGlowChanged,
          [&](StyleSheetProperties::GlowEffect *playerGlowEffect) {
            playerShadowEffect->setColor(playerGlowEffect->color);
            playerShadowEffect->setBlurRadius(playerGlowEffect->radius);
            playerShadowEffect->setXOffset(playerGlowEffect->xOffset);
            playerShadowEffect->setYOffset(playerGlowEffect->yOffset);
          });
  connect(
      theme_, &Theme::themeVolumepopupGlowChanged,
      [&](StyleSheetProperties::GlowEffect *volumepopupGlowEffect) {
        volumepopupShadowEffect->setColor(volumepopupGlowEffect->color);
        volumepopupShadowEffect->setBlurRadius(volumepopupGlowEffect->radius);
        volumepopupShadowEffect->setXOffset(volumepopupGlowEffect->xOffset);
        volumepopupShadowEffect->setYOffset(volumepopupGlowEffect->yOffset);
      });
  connect(theme_, &Theme::themePlayerWidgetChanged,
          [&](QString stylesheet) { mainWidget->setStyleSheet(stylesheet); });
  connect(theme_, &Theme::themeVolumepopupWidgetChanged,
          [&](StyleSheetProperties::VolumePopupWidget *volumepopupWidget) {
            VolumePopup::blur_padding = volumepopupWidget->blurPadding;
            VolumePopup::edge_curve = volumepopupWidget->edgeCurve;
            VolumePopup::widget_layout_padding =
                volumepopupWidget->layoutPadding;
            volume_popup->setWidgetColor(volumepopupWidget->backgroundColor);
            volume_popup->redrawSliderWidget();
          });
  connect(theme_, &Theme::themeTrackSliderWidgetChanged,
          [&](QString stylesheet) { track_slider->setStyleSheet(stylesheet); });
  connect(theme_, &Theme::themeVolumeSliderWidgetChanged, volume_popup,
          &VolumePopup::setVolumeSliderStylesheet);
  connect(theme_, &Theme::themeVscrollbarChanged, [&](QString stylesheet) {
    folder_view_->verticalScrollBar()->setStyleSheet(stylesheet);
    metadata_widget->verticalScrollBar()->setStyleSheet(stylesheet);
    console_widget_->setConsoleStylesheetScrollbar(stylesheet);
    playlist_view->verticalScrollBar()->setStyleSheet(stylesheet);
    library_view_->verticalScrollBar()->setStyleSheet(stylesheet);
  });
  connect(
      theme_, &Theme::themeCurrentSongMetadataLabelWidgetChanged,
      [&](QString stylesheet) { metadata_widget->setStyleSheet(stylesheet); });
  connect(theme_, &Theme::themeTimeLabelWidgetChanged,
          [&](QString stylesheet) { timer_label->setStyleSheet(stylesheet); });
  connect(
      theme_, &Theme::themePlaylistviewWidgetChanged,
      [&](QString stylesheet) { playlist_view->setStyleSheet(stylesheet); });
  connect(theme_, &Theme::themeLibraryviewWidgetChanged,
          [&](QString stylesheet) { library_view_->setStyleSheet(stylesheet); });
  connect(theme_, &Theme::themeFolderviewWidgetChanged,
          [&](QString stylesheet) { folder_view_->setStyleSheet(stylesheet); });
  connect(theme_, &Theme::themeConsoleWidgetChanged, [&](QString stylesheet) {
    console_widget_->setStyleSheet(stylesheet);
  });
  theme_->setTheme(Theme::Themes::TodiDark);
  // Button stylesheet acordingly
  close_pushButton->setStyleSheet("QPushButton{border: none;}");
  expand_collapse_PushButton->setStyleSheet("QPushButton{border: none;}");
  previous_pushButton->setStyleSheet("QPushButton{border: none;}");
  play_pause_pushButton->setStyleSheet("QPushButton{border: none;}");
  next_pushButton->setStyleSheet("QPushButton{border: none;}");
  volume_pushButton->setStyleSheet("QPushButton{border: none;}");
  search_pushButton->setStyleSheet("QPushButton{border: none;}");
  playlist_pushButton->setStyleSheet("QPushButton{border: none;}");
  fancy_tab_widget->setStyleSheet(
      ".QWidget{border-radius: 0px; background-color: rgba(155, 70, 70, 225); "
      "border: 0px solid #5c5c5c;}");

  // set all graphics effects here
  setGraphicsEffect(playerShadowEffect);
  this->setGraphicsEffect(playerShadowEffect);
  volume_popup->setGraphicsEffect(volumepopupShadowEffect);

  // initialize iconLoader values
  IconLoader::init();
  IconLoader::lumen_ = IconLoader::isLight(Qt::black);

  qInfo() << "Starting Todi application...";

  // Set icons
  close_pushButton->setIcon(
      IconLoader::load("edit-close", IconLoader::LightDark));
  expand_collapse_PushButton->setIcon(
      IconLoader::load("edit-collapse", IconLoader::LightDark));
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

  playlist_view->setItemDelegate(new CurrentPlaylistViewDeligate());
  playlist_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
  playlist_view->setWrapping(false);
  playlist_view->setLayoutMode(QListView::Batched);
  playlist_view->setBatchSize(500);
  playlist_view->setFlow(QListView::TopToBottom);
  playlist_view->setSelectionRectVisible(true);

  QGridLayout *baselayout = new QGridLayout(this);
  baselayout->setContentsMargins(5, 5, 5, 5);
  baselayout->addWidget(mainWidget);

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

  QHBoxLayout *hboxfinal = new QHBoxLayout();
  // hboxfinal->setContentsMargins(3, 3, 0, 3);
  hboxfinal->setContentsMargins(0, 0, 0, 0);
  hboxfinal->setSpacing(5);
  // hboxfinal->addWidget(new QSizeGrip(widget), 0, Qt::AlignBottom |
  // Qt::AlignLeft);

  currentSongMetadata_label->hide();
  hboxfinal->addLayout(expandcollapsecloselayout, 0);
  hboxfinal->addWidget(currentCoverArt_label, 1);
  hboxfinal->addWidget(currentSongMetadata_label, 2);
  hboxfinal->addLayout(vboxplaypauseslidertimer, 3);
  // hboxfinal->addWidget(new QSizeGrip(widget), 0, Qt::AlignBottom |
  // Qt::AlignRight);
  // miniplayerWidget->setLayout(hboxfinal);
  // mainWidget->setFixedHeight(48);

  QHBoxLayout *hboxfancy = new QHBoxLayout();
  fancy_tab_widget->AddTab(
      playlist_view,
      IconLoader::load("view-media-currentlist", IconLoader::LightDark),
      "Qeue");
  fancy_tab_widget->AddTab(
      library_view_,
      IconLoader::load("view-media-playlist", IconLoader::LightDark),
      "Library");
  fancy_tab_widget->AddTab(
      folder_view_,
      IconLoader::load("view-media-folder", IconLoader::LightDark), "Folders");
  fancy_tab_widget->AddTab(
      metadata_widget,
      IconLoader::load("view-media-metadata", IconLoader::LightDark),
      "Metadata");
  fancy_tab_widget->AddSpacer(5);
  fancy_tab_widget->AddTab(
      console_widget_, IconLoader::load("view-console", IconLoader::LightDark),
      "Console");
  fancy_tab_widget->SetMode(FancyTabWidget::Mode::Mode_LargeSidebar);
  fancy_tab_widget->SetCurrentIndex(0);
  hboxfancy->setContentsMargins(0, 0, 0, 0);
  hboxfancy->setSpacing(0);
  hboxfancy->addWidget(fancy_tab_widget, 0);
  hboxfancy->addWidget(stack_widget, 1);
  stack_widget->addWidget(playlist_view);
  stack_widget->addWidget(library_view_);
  stack_widget->addWidget(folder_view_);
  stack_widget->addWidget(metadata_widget);
  stack_widget->addWidget(console_widget_);

  QVBoxLayout *vboxfinal = new QVBoxLayout(mainWidget);
  vboxfinal->setContentsMargins(0, 0, 0, 0);
  vboxfinal->setSpacing(0);
  vboxfinal->addLayout(hboxfinal, 0);
  vboxfinal->addLayout(hboxfancy, 1);

  mainWidget->layout()->setSizeConstraint(QLayout::SetNoConstraint);
  timer_label->setText("--:--");

  currentCoverArt_label->setFixedWidth(48);
  currentCoverArt_label->setFixedHeight(48);

  currentPlaylistModel_ =
      new CurrentPlaylistModel(dataAccess_->getPlaylistinfoValues());
  playlist_view->setModel(currentPlaylistModel_);

  // update folder browse view
  filemodel_ = new FileModel(folder_view_, dataAccess_->getListallValues());
  folder_view_->setModel(filemodel_);
  folder_view_->header()->hide();

  librarymodel_ = new LibraryModel(library_view_);
  library_view_->setModel(librarymodel_);

  this->setMouseTracking(true);

  mainWidget->installEventFilter(this);
  mainWidget->setMouseTracking(true);
  timer_label->installEventFilter(this);
  timer_label->setMouseTracking(true);
  playlist_view->viewport()->installEventFilter(this);
  playlist_view->viewport()->setMouseTracking(true);

  // Set connection data
  QSettings settings;

  // Tray stuf
  setupTrayIcon();

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

  auto quitApplication = [=]() {
    QSettings settings;
    settings.beginGroup("player");
    settings.setValue("geometry", saveGeometry());
    settings.endGroup();
    QApplication::quit();
  };

  auto aboutApplication = []() {
    std::unique_ptr<AboutDialog> abt(new AboutDialog());
    abt->exec();
  };

  auto showMetadataSlingshot = [=]() {
    if (!volume_popup->isVisible()) {
      previous_pushButton->hide();
      play_pause_pushButton->hide();
      next_pushButton->hide();
      volume_pushButton->hide();
      track_slider->hide();
      timer_label->hide();
      // these two lines solve First time player resize showing
      // currentSongMetadata_label(need to fix)
      currentSongMetadata_label->setFixedWidth(track_slider->width() +
                                               timer_label->width());
      currentSongMetadata_label->updateSongMetadataText(false);
      currentSongMetadata_label->show();
      show_metadata_on_mouse_leave_ = true;
    }
  };

  // Console widget
  connect(mpdClient_, &MPDClient::commandsent, console_widget_,
          &ConsoleWidget::commandwithresults);
  connect(console_widget_, &ConsoleWidget::sendCommand, mpdClient_,
          &MPDClient::sendcommand);

  // Fancy tab widget tab changes
  connect(fancy_tab_widget, &FancyTabWidget::CurrentChanged,
          [&](int tab) { stack_widget->setCurrentIndex(tab); });

  // MPD
  connect(dataAccess_.get(), &MPDdata::MPDStatsUpdated, this,
          &Player::updateStats);
  connect(dataAccess_.get(), &MPDdata::MPDStatusUpdated, this,
          &Player::updateStatus, Qt::DirectConnection);

  connect(showSongMetadataAction, &QAction::triggered, this,
          &Player::showCurrentSongMetadata);
  connect(quitAction, &QAction::triggered, quitApplication);
  connect(close_pushButton, &QPushButton::clicked,
          [=]() { (trayIcon->isVisible()) ? hide() : QApplication::quit(); });
  connect(aboutAction, &QAction::triggered, aboutApplication);
  connect(trayIcon, &SystemTrayIcon::activated, this,
          [=]() { (isHidden()) ? show() : hide(); });
  connect(expand_collapse_PushButton, &QPushButton::clicked, this,
          &Player::expandCollapse);

  // Basic media buttons
  connect(play_pause_pushButton, &QPushButton::clicked, this,
          &Player::playPauseTrack);
  connect(previous_pushButton, &QPushButton::clicked, [&]() {
    playbackCtrlr_->previous();
    dataAccess_->getMPDStatus();
  });
  connect(next_pushButton, &QPushButton::clicked, [=]() {
    playbackCtrlr_->next();
    dataAccess_->getMPDStatus();
  });
  connect(consumeAction, &QAction::toggled,
          [=](bool status) { playbackOptionsCtrlr_->consume(status); });

  // volume & track slider
  connect(track_slider, &TrackSlider::sliderPressed, this,
          &Player::positionSliderPressed);
  connect(track_slider, &TrackSlider::sliderReleased, this,
          &Player::setPosition);
  connect(track_slider, &TrackSlider::sliderReleased, this,
          &Player::positionSliderReleased);
  connect(track_slider, &TrackSlider::valueChanged, [=](const int value) {
    trayIcon->trayIconUpdateProgress(value, track_slider->maximum());
  });
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
  connect(dataAccess_.get(), &MPDdata::MPDSongMetadataUpdated,
          currentArtLoader_, &CurrentArtLoader::loadCoverArt,
          Qt::QueuedConnection);

  // Update MetadataWidget
  connect(dataAccess_.get(), &MPDdata::MPDSongMetadataUpdated, [&]() {
    metadata_widget->setMetadata(dataAccess_->getSongMetadataValues());
  });

  // Update Folder View
  connect(dataAccess_.get(), &MPDdata::MPDListallUpdated, filemodel_,
          &FileModel::ViewUpdated);

  // update current playlist model
  connect(dataAccess_.get(), &MPDdata::MPDPlaylistinfoUpdated,
          currentPlaylistModel_, &CurrentPlaylistModel::updateModel);
  connect(this, &Player::songChanged, currentPlaylistModel_,
          &CurrentPlaylistModel::updateCurrentSong);
  connect(playlist_view, &QListView::doubleClicked, currentPlaylistModel_,
          &CurrentPlaylistModel::doubleClicked);
  connect(currentPlaylistModel_, &CurrentPlaylistModel::playSong,
          [=](const quint32 song) { playbackCtrlr_->play(song); });
  // metadata single slingshot
  QTimer::singleShot(3000, this, showMetadataSlingshot);

  // update status & stats when starting the application
  dataAccess_->getMPDStatus();
  dataAccess_->getMPDStats();
  dataAccess_->getMPDListall();
  dataAccess_->getMPDPlaylistInfo();
  dataAccess_->getMPDLibrary();
  librarymodel_->updateLibrary(dataAccess_->getLibraryValues());
}

QSize Player::sizeHint() const { return QSize(100, 40); }

Player::~Player() { delete ui_; }

void Player::mousePressEvent(QMouseEvent *event) {
  dragPosition = event->pos();
  if (event->button() == Qt::LeftButton &&
      cursor().shape() != Qt::SizeFDiagCursor) {
    setCursor(Qt::SizeAllCursor);
  }
  QWidget::mousePressEvent(event);
}

void Player::mouseReleaseEvent(QMouseEvent *event) {
  dragPosition = QPoint();
  if (event->button() == Qt::LeftButton) {
    setCursor(Qt::ArrowCursor);
  }
  QWidget::mouseReleaseEvent(event);
}

void Player::leaveEvent(QEvent *) {
  if (!volume_popup->isVisible() && show_metadata_on_mouse_leave_) {
    previous_pushButton->hide();
    play_pause_pushButton->hide();
    next_pushButton->hide();
    volume_pushButton->hide();
    track_slider->hide();
    timer_label->hide();
    // these two lines solve First time player resize showing
    // currentSongMetadata_label(need to fix)
    /*currentSongMetadata_label->setFixedWidth(track_slider->width() +
                                             timer_label->width());
    currentSongMetadata_label->updateSongMetadataText(false);*/
    currentSongMetadata_label->show();
  }
}

void Player::enterEvent(QEvent *) {
  currentSongMetadata_label->hide();
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
  if (mainWidget->width() > 200) {
    currentCoverArt_label->show();
  } else {
    currentCoverArt_label->hide();
  }
  if (mainWidget->width() > 350) {
    search_pushButton->show();
    playlist_pushButton->show();
  } else {
    search_pushButton->hide();
    playlist_pushButton->hide();
  }

  currentSongMetadata_label->setFixedWidth(track_slider->width() +
                                           timer_label->width());
  // songMetadata_label->setFixedWidth(layout->contentsRect().width() - 10);
  // while resizing we dont want animation (so pass false as arg)
  currentSongMetadata_label->updateSongMetadataText(false);
}

bool Player::eventFilter(QObject *target, QEvent *event) {
  if (target == mainWidget) {
    if (event->type() == QEvent::MouseMove) {
    }
  } else if (target == playlist_view->viewport()) {
    if (event->type() == QEvent::MouseMove) {
      if (cursor().shape() == Qt::SizeFDiagCursor) {
        setCursor(Qt::ArrowCursor);
        resize_status = false;
      }
    }
  }
  return QWidget::eventFilter(target, event);
}

int Player::showMpdConnectionDialog() {
  std::unique_ptr<MpdConnectionDialog> connect(new MpdConnectionDialog(this));
  return connect->exec();
}

void Player::expandCollapse() {
  if (!stack_widget->isVisible()) {
    expand_collapse_PushButton->setIcon(
        IconLoader::load("edit-collapse", IconLoader::LightDark));
    setFixedHeight(fullHeight_);
    stack_widget->setVisible(true);
    fancy_tab_widget->setVisible(true);
    // method to reverse fixed height
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    setMinimumSize(0, 0);
  } else {
    fullHeight_ = height();
    collapsedHeight_ = height() - stack_widget->height();
    expand_collapse_PushButton->setIcon(
        IconLoader::load("edit-expand", IconLoader::LightDark));
    stack_widget->setVisible(false);
    fancy_tab_widget->setVisible(false);
    setFixedHeight(collapsedHeight_);
  }
}

void Player::showVolumeSlider() {
  QPoint position = volume_pushButton->pos();
  // Position volume popup window accordingly
  position.setX(position.x() + (volume_pushButton->width() / 2));
  position.setX(position.x() - (volume_popup->width() / 2));
  position.setY(position.y() + (volume_pushButton->height() / 2));
  position.setY(position.y() -
                (volume_popup->height() - (VolumePopup::blur_padding * 2)));
  volume_popup->move(mapToGlobal(position));
  volume_popup->show();
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
    emit songChanged(dataAccess_->song() + 1);
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

  // Check if playlist has changed and update if needed
  if (lastState == MPDPlaybackState::Inactive ||
      lastPlaylist < dataAccess_->playlist()) {
    dataAccess_->getMPDPlaylistInfo();
  }

  // Display bitrate
  bitrateLabel.setText("Bitrate: " + QString::number(dataAccess_->bitrate()));

  // Update status info
  lastState = dataAccess_->state();
  lastSongId = dataAccess_->songId();
  lastPlaylist = dataAccess_->playlist();
}

void Player::playPauseTrack() const {
  if (dataAccess_->state() == MPDPlaybackState::Playing) {
    playbackCtrlr_->pause(1);
  } else if (dataAccess_->state() == MPDPlaybackState::Paused) {
    playbackCtrlr_->pause(0);
  } else {
    if (!playbackCtrlr_->playId(lastSongId)) {
      playbackCtrlr_->play(0);
    }
  }

  dataAccess_->getMPDStatus();
}

void Player::stopTrack() const {
  playbackCtrlr_->stop();
  dataAccess_->getMPDStatus();
}

void Player::positionSliderPressed() { draggingPositionSlider = true; }

void Player::setPosition() const {
  playbackCtrlr_->seekId(static_cast<quint32>(dataAccess_->songId()),
                         static_cast<quint32>(track_slider->value()));
  dataAccess_->getMPDStatus();
}

void Player::seekBackward() const {
  playbackCtrlr_->seekId(static_cast<quint32>(dataAccess_->songId()),
                         static_cast<quint32>(track_slider->value() - 10));
  dataAccess_->getMPDStatus();
}

void Player::seekForward() const {
  playbackCtrlr_->seekId(static_cast<quint32>(dataAccess_->songId()),
                         static_cast<quint32>(track_slider->value() + 10));
  dataAccess_->getMPDStatus();
}

void Player::positionSliderReleased() { draggingPositionSlider = false; }

void Player::setVolume(const quint8 value) const {
  playbackOptionsCtrlr_->setvol(value);
}

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

  if (!trayIcon) {
    trayIcon = new SystemTrayIcon(this);
  } else {
    return trayIcon;
  }

  trayIcon->show();
  return true;
}
