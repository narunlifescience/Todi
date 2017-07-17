#include "tagreader.h"

#include <aifffile.h>
#include <asffile.h>
#include <attachedpictureframe.h>
#include <commentsframe.h>
#include <fileref.h>
#include <flacfile.h>
#include <id3v2tag.h>
#include <mp4file.h>
#include <mp4tag.h>
#include <mpcfile.h>
#include <mpegfile.h>
#include <oggfile.h>
#include <oggflacfile.h>
#include <opusfile.h>
#include <popularimeterframe.h>
#include <speexfile.h>
#include <tag.h>
#include <textidentificationframe.h>
#include <trueaudiofile.h>
#include <tstring.h>
#include <unsynchronizedlyricsframe.h>
#include <vorbisfile.h>
#include <wavfile.h>

#include <QDebug>
#include <QDir>
#include <QFile>

TagReader::TagReader(QObject* parent) : QObject(parent) {}

void TagReader::loadCoverArt(QString filename) {
  filename = filename.trimmed();
  filename = "/home/arun/Music/" + filename;
  QByteArray bytearray = loadEmbededArt(filename);
  QImage image;
  (bytearray == QByteArray()) ? image.load(":/icons/nocover.png")
                              : image.loadFromData(bytearray);
  coverArtProcessed(image);
}

QByteArray TagReader::loadEmbededArt(QString filename) {
  if (filename.isEmpty()) return QByteArray();

#ifdef Q_OS_WIN32
  TagLib::FileRef ref(filename.toStdWString().c_str());
#else
  TagLib::FileRef ref(QFile::encodeName(filename).constData());
#endif

  if (ref.isNull() || !ref.file()) {
    return QByteArray();
  }

  // MP3
  TagLib::MPEG::File* file = dynamic_cast<TagLib::MPEG::File*>(ref.file());
  if (file && file->ID3v2Tag()) {
    TagLib::ID3v2::FrameList apic_frames =
        file->ID3v2Tag()->frameListMap()["APIC"];
    if (apic_frames.isEmpty()) {
      return QByteArray();
    }

    TagLib::ID3v2::AttachedPictureFrame* pic =
        static_cast<TagLib::ID3v2::AttachedPictureFrame*>(apic_frames.front());

    return QByteArray((const char*)pic->picture().data(),
                      pic->picture().size());
  }

  // Ogg vorbis/speex
  TagLib::Ogg::XiphComment* xiph_comment =
      dynamic_cast<TagLib::Ogg::XiphComment*>(ref.file()->tag());

  if (xiph_comment) {
    TagLib::Ogg::FieldListMap map = xiph_comment->fieldListMap();
    TagLib::List<TagLib::FLAC::Picture*> pics = xiph_comment->pictureList();
    if (!pics.isEmpty()) {
      for (auto p : pics) {
        if (p->type() == TagLib::FLAC::Picture::FrontCover)
          return QByteArray(p->data().data(), p->data().size());
      }
      // If there was no specific front cover, just take the first picture
      std::list<TagLib::FLAC::Picture*>::iterator it = pics.begin();
      TagLib::FLAC::Picture* picture = *it;

      return QByteArray(picture->data().data(), picture->data().size());
    }
    // Ogg lacks a definitive standard for embedding cover art, but it seems
    // b64 encoding a field called COVERART is the general convention
    if (!map.contains("COVERART")) return QByteArray();

    return QByteArray::fromBase64(map["COVERART"].toString().toCString());
  }

  // Flac
  TagLib::FLAC::File* flac_file = dynamic_cast<TagLib::FLAC::File*>(ref.file());
  if (flac_file && flac_file->xiphComment()) {
    TagLib::List<TagLib::FLAC::Picture*> pics = flac_file->pictureList();
    if (!pics.isEmpty()) {
      // Use the first picture in the file - this could be made cleverer and
      // pick the front cover if it's present.

      std::list<TagLib::FLAC::Picture*>::iterator it = pics.begin();
      TagLib::FLAC::Picture* picture = *it;

      return QByteArray(picture->data().data(), picture->data().size());
    }
  }

  // MP4/AAC
  TagLib::MP4::File* aac_file = dynamic_cast<TagLib::MP4::File*>(ref.file());
  if (aac_file) {
    TagLib::MP4::Tag* tag = aac_file->tag();
    const TagLib::MP4::ItemListMap& items = tag->itemListMap();
    TagLib::MP4::ItemListMap::ConstIterator it = items.find("covr");
    if (it != items.end()) {
      const TagLib::MP4::CoverArtList& art_list = it->second.toCoverArtList();

      if (!art_list.isEmpty()) {
        // Just take the first one for now
        const TagLib::MP4::CoverArt& art = art_list.front();
        return QByteArray(art.data().data(), art.data().size());
      }
    }
  }

  // any other case
  qInfo() << "unable to load coverart: "
          << QFile::encodeName(filename).constData();
  return QByteArray();
}

TagReader::TagReaderFileType TagReader::guessAudioFileType(
    TagLib::FileRef* fileref) const {
  if (dynamic_cast<TagLib::ASF::File*>(fileref->file()))
    return TagReaderFileType::Type_ASF;
  if (dynamic_cast<TagLib::FLAC::File*>(fileref->file()))
    return TagReaderFileType::Type_FLAC;
  if (dynamic_cast<TagLib::MP4::File*>(fileref->file()))
    return TagReaderFileType::Type_MP4;
  if (dynamic_cast<TagLib::MPC::File*>(fileref->file()))
    return TagReaderFileType::Type_MPC;
  if (dynamic_cast<TagLib::MPEG::File*>(fileref->file()))
    return TagReaderFileType::Type_MPEG;
  if (dynamic_cast<TagLib::Ogg::FLAC::File*>(fileref->file()))
    return TagReaderFileType::Type_OGGFLAC;
  if (dynamic_cast<TagLib::Ogg::Speex::File*>(fileref->file()))
    return TagReaderFileType::Type_OGGSPEEX;
  if (dynamic_cast<TagLib::Ogg::Vorbis::File*>(fileref->file()))
    return TagReaderFileType::Type_OGGVORBIS;
  if (dynamic_cast<TagLib::Ogg::Opus::File*>(fileref->file()))
    return TagReaderFileType::Type_OGGOPUS;
  if (dynamic_cast<TagLib::RIFF::AIFF::File*>(fileref->file()))
    return TagReaderFileType::Type_AIFF;
  if (dynamic_cast<TagLib::RIFF::WAV::File*>(fileref->file()))
    return TagReaderFileType::Type_WAV;
  if (dynamic_cast<TagLib::TrueAudio::File*>(fileref->file()))
    return TagReaderFileType::Type_TRUEAUDIO;

  return TagReaderFileType::Type_UNKNOWN;
}

bool TagReader::isJpg(const QByteArray& data) {
  return data.size() > 9 && data[6] == 'J' && data[7] == 'F' &&
         data[8] == 'I' && data[9] == 'F';
}

bool TagReader::isPng(const QByteArray& data) {
  return data.size() > 4 && data[1] == 'P' && data[2] == 'N' && data[3] == 'G';
}
