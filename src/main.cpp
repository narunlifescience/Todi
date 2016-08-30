#include <QApplication>
#include "IconLoader.h"
#include "Player.h"

int main(int argc, char *argv[]) {
  IconLoader::init();
  QApplication a(argc, argv);
  Player w;
  w.show();
  return a.exec();
}
