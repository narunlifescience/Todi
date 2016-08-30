#ifndef MPRISHANDLER_H
#define MPRISHANDLER_H

class MprisHandler {
 public:
  virtual ~MprisHandler() {}

  enum class PlayBack { Play, Pause, PlayPause, Next, Previous, Stop };

  enum class PlayBackStatus {
    InvalidPlaybackStatus = -1,
    Playing,
    Paused,
    Stopped
  };

  enum class LoopStatus { InvalidLoopStatus = -1, None, Track, Playlist };

  enum class Metadata {
    InvalidMetadata = -1,
    TrackId,
    Length,
    ArtUrl,
    Album,
    AlbumArtist,
    Artist,
    AsText,
    AudioBPM,
    AutoRating,
    Comment,
    Composer,
    ContentCreated,
    DiscNumber,
    FirstUsed,
    Genre,
    LastUsed,
    Lyricist,
    Title,
    TrackNumber,
    Url,
    UseCount,
    UserRating
  };

  virtual void MediaPlaybackNoArgs(MprisHandler::PlayBack platback) = 0;
  virtual MprisHandler::PlayBackStatus MediaPlaybackStatus() = 0;
  virtual void getPlayingAlbumCover() = 0;

  static MprisHandler* GetMprisPlayerHandle();
  static const char* ClementineMpris2Service;

 private:
  static MprisHandler* mprisPlayerHandle_;
};

#endif  // MPRISHANDLER_H
