#include <QString>

class Todi {
public :
  // Note: dont use these outside gui thread
  static QString hostname;
  static quint16 port;
  static QString passwd;
};
