#ifndef TOOLTIPWIDGET_H
#define TOOLTIPWIDGET_H

#include <QLabel>

class QVBoxLayout;

class TooltipWidget : public QLabel {
  Q_OBJECT
 public:
  TooltipWidget(QWidget *parent = nullptr);
  TooltipWidget *getInstance() {
    if (!tooltipwidget_) tooltipwidget_ = new TooltipWidget();

    return tooltipwidget_;
  }

  void SetTextTooltip(QString &text);
  void SetPixmapTooltip(QImage *image);

 private:
  TooltipWidget *tooltipwidget_;
  QLabel *texttip_;
  QLabel *pixmaptip_;
  QVBoxLayout *tiplayout_;
  QString text_;
  QImage *image_;

 public:
  // Dont forget to declare these two. You want to make sure they
  // are unacceptable otherwise you may accidentally get copies of
  // your singleton appearing (C++ 11)
  TooltipWidget(TooltipWidget const &) = delete;
  void operator=(TooltipWidget const &) = delete;
};

#endif  // TOOLTIPWIDGET_H
