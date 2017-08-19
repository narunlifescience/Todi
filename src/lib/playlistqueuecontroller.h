#ifndef CURRENTPLAYLISTCONTROLLER_H
#define CURRENTPLAYLISTCONTROLLER_H

#include <QObject>
#include <memory>

class MPDSocket;

class CurrentPlaylistController : QObject {
  Q_OBJECT
 public:
  CurrentPlaylistController(QObject *parent = nullptr,
                          std::shared_ptr<MPDSocket> mpdSocket = nullptr);
  ~CurrentPlaylistController();

public slots:
  bool playlistinfo() const;

private:
  std::shared_ptr<MPDSocket> mpdSocket_;
  const static QByteArray playlistinfoCmd;
};

#endif  // CURRENTPLAYLISTCONTROLLER_H
