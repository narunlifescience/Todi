#ifndef COVERLOADER_H
#define COVERLOADER_H

#include <QImage>
#include <QObject>

#include <fileref.h>
#include <taglib/taglib.h>

// class TagLib;
// class TagLib::FileRef;

class CurrentArtLoader : public QObject {
  Q_OBJECT
 public:
  explicit CurrentArtLoader(QObject *parent = nullptr);
  enum class TagReaderFileType {
    Type_ASF,
    Type_FLAC,
    Type_MP4,
    Type_MPC,
    Type_MPEG,
    Type_OGGFLAC,
    Type_OGGSPEEX,
    Type_OGGVORBIS,
    Type_OGGOPUS,
    Type_AIFF,
    Type_WAV,
    Type_TRUEAUDIO,
    Type_UNKNOWN
  };
 signals:
  void coverArtProcessed(QImage *image) const;

 public slots:
  void loadCoverArt(QString filename);

 private:
  QByteArray loadEmbededArt(QString filename);
  TagReaderFileType guessAudioFileType(TagLib::FileRef *fileref) const;
  bool isJpg(const QByteArray &data);
  bool isPng(const QByteArray &data);
  QImage *image_;
};

#endif  // COVERLOADER_H
