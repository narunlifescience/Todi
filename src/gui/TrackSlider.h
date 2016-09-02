#ifndef TRACKSLIDER_H
#define TRACKSLIDER_H

#include <QMouseEvent>
#include <QSlider>
#include <QStyle>
#include <QStyleOptionSlider>

class TrackSlider : public QSlider {
 public:
  TrackSlider(Qt::Orientation orientation, QWidget* parent = nullptr)
      : QSlider(orientation, parent) {}
  ~TrackSlider() {}

 protected:
  void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE {
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt,
                                       QStyle::SC_SliderHandle, this);

    if (event->button() == Qt::LeftButton && !sr.contains(event->pos())) {
      int newVal;
      double halfHandleWidth = (0.5 * sr.width()) + 0.5;
      int adaptedPosX = event->x();
      if (adaptedPosX < halfHandleWidth)
        adaptedPosX = static_cast<int>(halfHandleWidth);
      if (adaptedPosX > width() - halfHandleWidth)
        adaptedPosX = static_cast<int>(width() - halfHandleWidth);
      double newWidth = (width() - halfHandleWidth) - halfHandleWidth;
      double normalizedPosition = (adaptedPosX - halfHandleWidth) / newWidth;
      newVal = static_cast<int>(minimum() +
                                ((maximum() - minimum()) * normalizedPosition));

      if (invertedAppearance()) {
        setValue(maximum() - newVal);
      } else {
        setValue(newVal);
      }

      event->accept();
    }
    QSlider::mousePressEvent(event);
  }
};

#endif  // TRACKSLIDER_H
