#include "mainwidget.h"

#include <QStyleOption>
#include <QStylePainter>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent) {
  //setAttribute(Qt::WA_TransparentForMouseEvents);
}

void MainWidget::paintEvent(QPaintEvent *event)
{
  QStyleOption opt;
  opt.init(this);
  QStylePainter p(this);
  p.drawPrimitive(QStyle::PE_Widget, opt);
  QWidget::paintEvent(event);
}
