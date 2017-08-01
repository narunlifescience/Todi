#include "currentplaylistcontroller.h"
#include "mpdsocket.h"

const QByteArray CurrentPlaylistController::clearCmd = "clear";

CurrentPlaylistController::CurrentPlaylistController(
    QObject *parent, std::shared_ptr<MPDSocket> mpdSocket)
    : QObject(parent), mpdSocket_(mpdSocket) {}

CurrentPlaylistController::~CurrentPlaylistController() {}

bool CurrentPlaylistController::clear() const {
  return mpdSocket_->sendCommand(clearCmd).second;
}
