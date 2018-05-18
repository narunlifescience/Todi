#ifndef MPDDATAPARSER_H
#define MPDDATAPARSER_H
#include "mpdmodel.h"

class RootItem;

namespace MPDdataParser {
void parseStatus(const QByteArray &data, MPDStatusValues *statusValues);
void parseStats(const QByteArray &data, MPDStatsValues *statsValues);
void parseSongMetadata(const QList<QByteArray> &data,
                       MPDSongMetadata *songMetadataValues);
void parsePlaylistQueue(const QByteArray &data,
                        QList<MPDSongMetadata *> *playlistQueue);
void parseFolderView(const QByteArray &data, RootItem *rootitem);
}

#endif  // MPDDATAPARSER_H
