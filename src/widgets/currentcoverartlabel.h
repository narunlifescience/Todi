#ifndef COVERART_H
#define COVERART_H

#include <QLabel>
#include "mpdmodel.h"

class Application;
class QBuffer;

class CurrentCoverArtLabel : public QLabel
{
  Q_OBJECT
public:
  explicit CurrentCoverArtLabel(Application *app, QWidget *parent = nullptr);

public slots:
  void setCoverArt(QImage *image, MPDSongMetadata* songmetadata);
  void setCoverArtTooltip();

protected:
  void mouseMoveEvent(QMouseEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);

private:
  void updateCoverArt();
  Application *app_;
  QImage *image_;
  MPDSongMetadata *songmetadata_;
  QByteArray *imageByteArray_;
  QBuffer *imageBuffer_;
  QString tooltiptext_;
  bool mousepressed_;
};

#endif // COVERART_H
