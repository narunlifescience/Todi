#ifndef MPDDATAPARSER_H
#define MPDDATAPARSER_H
#include "mpdmodel.h"

namespace MPDdataParser {
void parseStatus(const QByteArray &data, MPDStatusValues *statusValues);
void parseStats(const QByteArray &data, MPDStatsValues *statsValues);
void parseSongMetadata(const QList<QByteArray> &data,
                       MPDSongMetadata *songMetadataValues);
void parsePlaylistQueue(const QByteArray &data,
                        QList<MPDSongMetadata *> *playlistQueue);
}

#endif  // MPDDATAPARSER_H
