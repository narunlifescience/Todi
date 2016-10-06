#ifndef MPDDATAPARSER_H
#define MPDDATAPARSER_H
#include "mpdmodel.h"

namespace MPDdataParser {
  MPDStatusValues parseStatus(const QByteArray &data);
  MPDStatsValues parseStats(const QByteArray &data);
}

#endif // MPDDATAPARSER_H
