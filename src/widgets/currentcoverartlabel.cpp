#include <QBuffer>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>
#include <QVBoxLayout>

#include "../core/application.h"
#include "../lib/mpdclient.h"
#include "../lib/mpddata.h"
#include "../tagger/currentartloader.h"
//#include "../tooltip/tooltip.h"
#include "currentcoverartlabel.h"

CurrentCoverArtLabel::CurrentCoverArtLabel(Application *app, QWidget *parent)
    : QLabel(parent),
      app_(app),
      image_(nullptr),
      imageByteArray_(new QByteArray()),
      imageBuffer_(new QBuffer(this)),
      mousepressed_(false),
      tipwidget_(new QWidget()),
      tipwidgetLayout_(new QVBoxLayout(tipwidget_)),
      texttipLabel_(new QLabel(tipwidget_)),
      pixmaptipLabel_(new QLabel(tipwidget_)) {
  setScaledContents(true);
  setMouseTracking(true);

  tipwidgetLayout_->addWidget(texttipLabel_);
  tipwidgetLayout_->addWidget(pixmaptipLabel_);
  tipwidgetLayout_->setContentsMargins(1, 1, 1, 1);

  setCoverArtAsTodi();

  connect(
      app_->currentArtLoader(), &CurrentArtLoader::coverArtProcessed,
      [&](QImage *image) {
        setCoverArt(
            image,
            app_->mpdClient()->getSharedMPDdataPtr()->getSongMetadataValues());
      });
}

CurrentCoverArtLabel::~CurrentCoverArtLabel() {
  delete tipwidget_;
  tipwidget_ = nullptr;
}

void CurrentCoverArtLabel::setCoverArt(QImage *image,
                                       MPDSongMetadata *songmetadata) {
  image_ = image;
  songmetadata_ = songmetadata;

  imageByteArray_->clear();
  imageBuffer_->setBuffer(imageByteArray_);
  imageBuffer_->open(QIODevice::WriteOnly);
  image_->save(imageBuffer_, "PNG", 100);
  imageBuffer_->close();

  updateCoverArt();
  setCoverArtTooltip();
}

void CurrentCoverArtLabel::setCoverArtAsTodi() {
  setPixmap(QPixmap(":/icons/todi.svg"));
}

void CurrentCoverArtLabel::setCoverArtTooltip() {
  if (songmetadata_) {
    tooltiptext_ = QString("<table>");
    tooltiptext_ +=
        QString(
            "<tr><td align=\"right\"><b>Artist:</b></td><td>%1</td></tr>"
            "<tr><td align=\"right\"><b>Album:</b></td><td>%2</td></tr>"
            "<tr><td align=\"right\"><b>Year:</b></td><td>%3</td></tr>")
            .arg(songmetadata_->artist)
            .arg(songmetadata_->album)
            .arg(QString::number(songmetadata_->date));
    tooltiptext_ += "</table>";
  }
  tooltiptext_ += QString("<br/><img src=\"data:image/png;base64, %0\"/>")
                      .arg(QString(imageByteArray_->toBase64()));
}

void CurrentCoverArtLabel::updateCoverArt() {
  if (image_->isNull()) {
    setPixmap(QPixmap(":/icons/nocover.png"));
    return;
  }

  // Display image
  QPixmap pixmap = QPixmap::fromImage(*image_);
  pixmaptipLabel_->setPixmap(pixmap);
  pixmap = pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
  QPainter painter(&pixmap);
  QPen pen(QColor(242, 242, 242), 2);
  painter.setPen(pen);
  painter.drawRect(pixmap.rect());

  setPixmap(pixmap);
}

void CurrentCoverArtLabel::mouseMoveEvent(QMouseEvent *event) {
  if (!mousepressed_) {
    QToolTip::showText(event->globalPos(), tooltiptext_, this, rect());
    // Utils::ToolTip::show(event->globalPos(), tooltiptext_, this, "tooltip",
    //                     rect());
  }
  QWidget::mouseMoveEvent(event);
}

void CurrentCoverArtLabel::mousePressEvent(QMouseEvent *event) {
  mousepressed_ = true;
  QWidget::mousePressEvent(event);
}

void CurrentCoverArtLabel::mouseReleaseEvent(QMouseEvent *event) {
  mousepressed_ = false;
  QWidget::mouseReleaseEvent(event);
}
