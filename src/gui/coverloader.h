#ifndef COVERLOADER_H
#define COVERLOADER_H

#include <QImage>
#include <QObject>

class  CoverLoader: public QObject {
  Q_OBJECT
 public:
  explicit CoverLoader(QObject *parent = nullptr);

 signals:
  void coverArtProcessed(QImage image);

 public slots:
  void loadCoverArt(QString filename);

 private:
  QByteArray loadEmbededArt(QString filename);
  bool isJpg(const QByteArray &data);
  bool isPng(const QByteArray &data);
};

#endif  // COVERLOADER_H
