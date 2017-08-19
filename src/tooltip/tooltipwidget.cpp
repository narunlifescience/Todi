#include "tooltipwidget.h"
#include <QVBoxLayout>

void TooltipWidget::SetTextTooltip(QString &text) {
  texttip_->setText(text);
  text_ = text;
}

void TooltipWidget::SetPixmapTooltip(QImage *image) {
  pixmaptip_->setPixmap(QPixmap::fromImage(*image));
  image_ = image;
}

TooltipWidget::TooltipWidget(QWidget *parent)
    : QLabel(parent),
      texttip_(new QLabel(this)),
      pixmaptip_(new QLabel(this)),
      tiplayout_(new QVBoxLayout(this)),
      image_(nullptr) {
  tiplayout_->addWidget(texttip_);
  tiplayout_->addWidget(pixmaptip_);
  setLayout(tiplayout_);
}
