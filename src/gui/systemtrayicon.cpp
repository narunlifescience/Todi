#include "systemtrayicon.h"

#include <QAction>
#include <QDebug>
#include <QEvent>
#include <QMenu>
#include <QPixmapCache>
#include <QWheelEvent>

SystemTrayIcon::SystemTrayIcon(QObject *parent)
    : QSystemTrayIcon(parent),
      prevSystemTrayProgress_(SystemTrayProgress::EighthOctave),
      menu_(new QMenu),
      action_play_pause_(nullptr),
      action_stop_(nullptr),
      action_stop_after_this_track_(nullptr),
      action_mute_(nullptr) {
  installEventFilter(this);
  setIcon(QIcon(":icons/todi.svg"));
  setToolTip("Todi");
  connect(this, &SystemTrayIcon::activated, this, &SystemTrayIcon::Clicked);
}

SystemTrayIcon::~SystemTrayIcon() { delete menu_; }

bool SystemTrayIcon::setupTrayIcon() {
  // trayIcon->installEventFilter(volumeSliderEventHandler);
  /*  trayIconMenu = new QMenu(this);

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

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
    this,
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
    trayIcon->setContextMenu(trayIconMenu);

    return true;*/
}

bool SystemTrayIcon::eventFilter(QObject *object, QEvent *event) {
  Q_UNUSED(object);
  if (event->type() == QEvent::Wheel) {
    QWheelEvent *wevnt = static_cast<QWheelEvent *>(event);
    (wevnt->delta() > 0) ? emit SeekForward() : emit SeekBackward();
  }
  return false;
}

void SystemTrayIcon::setTrayIconProgress(
    SystemTrayIcon::SystemTrayProgress trayProgress) {
  QPixmap traypix;
  switch (trayProgress) {
    case SystemTrayProgress::FirstOctave:

      if (!QPixmapCache::find("onebyeight", &traypix)) {
        traypix.load(":/icons/tray/onebyeight.svg");
        QPixmapCache::insert("onebyeight", traypix);
      }
      setIcon(traypix);
      break;
    case SystemTrayProgress::SecondOctave:
      if (!QPixmapCache::find("twobyeight", &traypix)) {
        traypix.load(":/icons/tray/twobyeight.svg");
        QPixmapCache::insert("twobyeight", traypix);
      }
      setIcon(traypix);
      break;
    case SystemTrayProgress::ThirdOctave:
      if (!QPixmapCache::find("threebyeight", &traypix)) {
        traypix.load(":/icons/tray/threebyeight.svg");
        QPixmapCache::insert("threebyeight", traypix);
      }
      setIcon(traypix);
      break;
    case SystemTrayProgress::FourthOctave:
      if (!QPixmapCache::find("fourbyeight", &traypix)) {
        traypix.load(":/icons/tray/fourbyeight.svg");
        QPixmapCache::insert("fourbyeight", traypix);
      }
      setIcon(traypix);
      break;
    case SystemTrayProgress::FifthOctave:
      if (!QPixmapCache::find("fivebyeight", &traypix)) {
        traypix.load(":/icons/tray/fivebyeight.svg");
        QPixmapCache::insert("fivebyeight", traypix);
      }
      setIcon(traypix);
      break;
    case SystemTrayProgress::SixthOctave:
      if (!QPixmapCache::find("sixbyeight", &traypix)) {
        traypix.load(":/icons/tray/sixbyeight.svg");
        QPixmapCache::insert("sixbyeight", traypix);
      }
      setIcon(traypix);
      break;
    case SystemTrayProgress::SeventhOctave:
      if (!QPixmapCache::find("sevenbyeight", &traypix)) {
        traypix.load(":/icons/tray/sevenbyeight.svg");
        QPixmapCache::insert("sevenbyeight", traypix);
      }
      setIcon(traypix);
      break;
    case SystemTrayProgress::EighthOctave:
      if (!QPixmapCache::find("eightbyeight", &traypix)) {
        traypix.load(":/icons/tray/eightbyeight.svg");
        QPixmapCache::insert("eightbyeight", traypix);
      }
      setIcon(traypix);
      break;
  }
  prevSystemTrayProgress_ = trayProgress;
}

void SystemTrayIcon::trayIconUpdateProgress(int value, int track_slider_max) {
  if (value != 0) {
    SystemTrayProgress trayProgress = SystemTrayProgress::EighthOctave;
    int percent = (value * 100) / track_slider_max;

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

    if (trayProgress != prevSystemTrayProgress_) {
      setTrayIconProgress(trayProgress);
    }

  } else {
    if (prevSystemTrayProgress_ != SystemTrayProgress::EighthOctave) {
      setTrayIconProgress(SystemTrayProgress::EighthOctave);
    }
  }
}

void SystemTrayIcon::Clicked(QSystemTrayIcon::ActivationReason reason) {
  switch (reason) {
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::Trigger:
      emit ShowHide();
      break;

    case QSystemTrayIcon::MiddleClick:
      emit PlayPause();
      break;

    case QSystemTrayIcon::Unknown:
    case QSystemTrayIcon::Context:
      break;
  }
}
