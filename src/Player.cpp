#include "Player.h"
#include "IconLoader.h"
#include "ui_Player.h"

#include <QLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QSizeGrip>
#include <QSpacerItem>
#include <QTimer>
#include <QWidget>

#include <QBitmap>
#include <QBrush>
#include <QImage>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>
#include <QStyle>
#include <QStyleOptionSlider>
#include <QTimer>
#include <QWheelEvent>

#include <QGraphicsDropShadowEffect>

Player::Player(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                          Qt::SubWindow),
      ui_(new Ui_Player),
      widget(new QWidget(this)),
      close_pushButton(new QPushButton(this)),
      expand_collapse_PushButton(new QPushButton(this)),
      previous_pushButton(new QPushButton(this)),
      play_pause_pushButton(new QPushButton(this)),
      next_pushButton(new QPushButton(this)),
      volume_pushButton(new QPushButton(this)),
      search_pushButton(new QPushButton(this)),
      playlist_pushButton(new QPushButton(this)),
      track_slider(new QSlider(Qt::Horizontal, this)),
      timer_label(new QLabel(this)),
      albumcover_label(new QLabel(this)),
      songMetadata_label(new QLabel(this)),
      volume_slider_frame(new QFrame(this, Qt::Popup)),
      volume_slider(new QSlider(Qt::Horizontal, volume_slider_frame)),
      resize_status(false),
      mprishandler_(MprisHandler::GetMprisPlayerHandle()) {
  ui_->setupUi(this);
  this->resize(this->width(), 61);

  IconLoader::lumen_ = IconLoader::isLight(Qt::black);
  this->setAttribute(Qt::WA_TranslucentBackground, true);
  setWindowTitle(tr("Todi"));

  QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
  effect->setColor(QColor(181, 185, 190));
  effect->setBlurRadius(5);
  effect->setXOffset(0);
  effect->setYOffset(0);
  setGraphicsEffect(effect);
  this->setGraphicsEffect(effect);
  // QTimer *timer = new QTimer(this);

  // Set icons
  close_pushButton->setIcon(IconLoader::load("edit-close", IconLoader::LightDark));
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

  // QSizeGrip *sizeGrip=new QSizeGrip(widget);

  QAction *quitAction = new QAction(tr("&Exit"), this);
  addAction(quitAction);
  setContextMenuPolicy(Qt::ActionsContextMenu);
  // quitAction->setShortcut(tr("Ctrl+Q"));

  connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(quitApplication()));
  connect(close_pushButton, SIGNAL(clicked(bool)), this,
          SLOT(quitApplication()));
  connect(expand_collapse_PushButton, SIGNAL(clicked(bool)), this,
          SLOT(expandCollapse()));
  connect(play_pause_pushButton, SIGNAL(clicked(bool)), this,
          SLOT(playPause()));
  connect(next_pushButton, SIGNAL(clicked(bool)), this, SLOT(next()));
  connect(previous_pushButton, SIGNAL(clicked(bool)), this, SLOT(previous()));

  // widget->setStyleSheet("QWidget{border-radius: 3px; background-color:
  // #31363b; border: 0px solid #5c5c5c;}");
  widget->setStyleSheet(
      ".QWidget{border-radius: 3px; background-color: rgba(20, 20, 20, 200); "
      "border: 0px solid #5c5c5c;}");
  track_slider->setStyleSheet(
      ".QSlider::groove:horizontal { border: 0px solid #999999; height: 3px; "
      "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, "
      "stop:1 #c4c4c4); "
      "margin: 0px 0; } .QSlider::handle:horizontal {"
      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eff0f1, "
      "stop:1 #eff0f1);"
      "border: 0px solid #5c5c5c;width: 2px;margin: -2px 0; }"
      ".QSlider::sub-page:horizontal {"
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

  // close_pushButton->setStyleSheet("QPushButton{border-radius: 5px;}");

  QGridLayout *baselayout = new QGridLayout(this);
  baselayout->addWidget(widget);

  QVBoxLayout *closecollapselayout = new QVBoxLayout;
  closecollapselayout->setContentsMargins(0, 3, 0, 3);
  closecollapselayout->addWidget(close_pushButton);
  closecollapselayout->addStretch();
  closecollapselayout->addWidget(expand_collapse_PushButton);

  QHBoxLayout *playerbuttonslayout = new QHBoxLayout;
  playerbuttonslayout->addWidget(previous_pushButton);
  playerbuttonslayout->addWidget(play_pause_pushButton);
  playerbuttonslayout->addWidget(next_pushButton);

  QHBoxLayout *slidertimerlayout = new QHBoxLayout;
  slidertimerlayout->setContentsMargins(0, 0, 0, 3);
  slidertimerlayout->setSpacing(3);
  slidertimerlayout->addWidget(track_slider);
  slidertimerlayout->addWidget(timer_label);
  // slidertimerlayout->addWidget(new QSizeGrip(widget), 0, Qt::AlignBottom |
  // Qt::AlignRight);

  QHBoxLayout *layout = new QHBoxLayout();
  layout->setContentsMargins(0, 3, 0, 0);
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
  hboxfinal->setContentsMargins(0, 0, 5, 0);
  hboxfinal->setSpacing(5);
  // hboxfinal->addWidget(new QSizeGrip(widget), 0, Qt::AlignBottom |
  // Qt::AlignLeft);
  hboxfinal->addLayout(closecollapselayout);
  hboxfinal->addWidget(albumcover_label);
  hboxfinal->addWidget(songMetadata_label);
  hboxfinal->addLayout(vboxplaypauseslidertimer);
  // hboxfinal->addWidget(new QSizeGrip(widget), 0, Qt::AlignBottom |
  // Qt::AlignRight);
  songMetadata_label->setVisible(false);
  songMetadata_label->setText("test Todi compact");

  widget->layout()->setSizeConstraint(QLayout::SetNoConstraint);
  timer_label->setText("--:--");
  QPixmap pixmap(":/icons/vtv.png");
  albumcover_label->setFixedWidth(this->height() - 20);
  albumcover_label->setFixedHeight(this->height() - 20);
  albumcover_label->resize(this->height() - 20, this->height() - 20);
  albumcover_label->setPixmap(
      pixmap.scaled(albumcover_label->size(), Qt::KeepAspectRatio));
  albumcover_label->setPixmap(pixmap.scaled(albumcover_label->size(),
                                            Qt::KeepAspectRatioByExpanding,
                                            Qt::SmoothTransformation));
  albumcover_label->setScaledContents(true);
  // slider->setMaximum(15);

  volume_slider_frame->resize(100, 40);

  connect(volume_pushButton, SIGNAL(clicked()), this, SLOT(showVolumeSlider()));
  QHBoxLayout *volumeslider = new QHBoxLayout(volume_slider_frame);
  volumeslider->addWidget(volume_slider);

  this->setMouseTracking(true);

  widget->installEventFilter(widget);
}

QSize Player::sizeHint() const { return QSize(100, 100); }

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

void Player::expandCollapse() {
  if (this->width() != this->height()) {
    this->setFixedHeight(this->width());
    expand_collapse_PushButton->setIcon(IconLoader::load("edit-collapse", IconLoader::LightDark));
  } else {
    this->setFixedHeight(61);
    // this->resize(this->width(), 61);
    expand_collapse_PushButton->setIcon(IconLoader::load("edit-expand", IconLoader::LightDark));
  }
}

void Player::showVolumeSlider() {
  // volume_slider_frame->setFrameStyle(3);
  // volume_slider_frame->setFrameShape(QFrame::StyledPanel);
  volume_slider_frame->show();
  // volume_slider_frame->move(volume_pushButton->x(), volume_pushButton->y());
  volume_slider_frame->move(
      mapToGlobal(volume_pushButton->geometry().topLeft()));
}

void Player::quitApplication() { QApplication::quit(); }

void Player::playPause() {
  if (mprishandler_) {
    mprishandler_->MediaPlaybackNoArgs(MprisHandler::PlayBack::PlayPause);
    setPlayPauseIcon();
  }
}

void Player::next() {
  if (mprishandler_) {
    mprishandler_->MediaPlaybackNoArgs(MprisHandler::PlayBack::Next);
  }
}

void Player::previous() {
  if (mprishandler_) {
    mprishandler_->MediaPlaybackNoArgs(MprisHandler::PlayBack::Previous);
  }
}

// set the play pause icon according to playback status
void Player::setPlayPauseIcon() {
  switch (mprishandler_->MediaPlaybackStatus()) {
    case MprisHandler::PlayBackStatus::Playing:
      play_pause_pushButton->setIcon(
          IconLoader::load("media-playback-pause", IconLoader::LightDark));
      break;
    case MprisHandler::PlayBackStatus::Paused:
      play_pause_pushButton->setIcon(
          IconLoader::load("media-playback-start", IconLoader::LightDark));
      break;
    case MprisHandler::PlayBackStatus::Stopped:
      play_pause_pushButton->setIcon(
          IconLoader::load("media-playback-start", IconLoader::LightDark));
      break;
    case MprisHandler::PlayBackStatus::InvalidPlaybackStatus:
      play_pause_pushButton->setIcon(
          IconLoader::load("media-playback-start", IconLoader::LightDark));
      break;
  }
}
