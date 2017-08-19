#ifndef COVERART_H
#define COVERART_H

#include <QLabel>
#include "mpdmodel.h"

class QVBoxLayout;
class QBuffer;
class Application;

class CurrentCoverArtLabel : public QLabel {
  Q_OBJECT
 public:
  explicit CurrentCoverArtLabel(Application *app, QWidget *parent = nullptr);
  ~CurrentCoverArtLabel();

 public slots:
  void setCoverArt(QImage *image, MPDSongMetadata *songmetadata);
  void setCoverArtTooltip();

 protected:
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

 private:
  void updateCoverArt();
  Application *app_;
  QImage *image_;
  MPDSongMetadata *songmetadata_;
  QString tooltiptext_;
  QByteArray *imageByteArray_;
  QBuffer *imageBuffer_;
  bool mousepressed_;
  QWidget *tipwidget_;
  QVBoxLayout *tipwidgetLayout_;
  QLabel *texttipLabel_;
  QLabel *pixmaptipLabel_;
};

#endif  // COVERART_H
