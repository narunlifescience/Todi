#ifndef MPDDATAPARSER_H
#define MPDDATAPARSER_H
#include "mpdmodel.h"

class RootItem;
class MusicLibraryItemArtist;

namespace MPDdataParser {
void parseStatus(const QByteArray &data, MPDStatusValues *statusValues);
void parseStats(const QByteArray &data, MPDStatsValues *statsValues);
void parseSongMetadata(const QList<QByteArray> &data,
                       MPDSongMetadata *songMetadataValues);
void parsePlaylistQueue(const QByteArray &data,
                        QList<MPDSongMetadata *> *playlistQueue);
void parseFolderView(const QByteArray &data, RootItem *rootitem);
void parseLibraryItems(const QByteArray &data,
                       QList<MusicLibraryItemArtist *> *artists);
}  // namespace MPDdataParser

#endif  // MPDDATAPARSER_H
