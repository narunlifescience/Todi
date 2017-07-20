#ifndef MPDDATAPARSER_H
#define MPDDATAPARSER_H
#include "mpdmodel.h"

namespace MPDdataParser {
void parseStatus(const QByteArray &data, MPDStatusValues *statusValues);
void parseStats(const QByteArray &data, MPDStatsValues *statsValues);
void parseSongMetadata(const QByteArray &data,
                       MPDSongMetadata *songMetadataValues);
}

#endif  // MPDDATAPARSER_H
