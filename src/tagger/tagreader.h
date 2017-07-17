#ifndef COVERLOADER_H
#define COVERLOADER_H

#include <QImage>
#include <QObject>

#include <taglib/taglib.h>
#include <fileref.h>

//class TagLib;
//class TagLib::FileRef;

class TagReader : public QObject {
  Q_OBJECT
 public:
  explicit TagReader(QObject *parent = nullptr);
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
  void coverArtProcessed(QImage image);

 public slots:
  void loadCoverArt(QString filename);

 private:
  QByteArray loadEmbededArt(QString filename);
  TagReaderFileType guessAudioFileType(TagLib::FileRef* fileref) const;
  bool isJpg(const QByteArray &data);
  bool isPng(const QByteArray &data);
};

#endif  // COVERLOADER_H
