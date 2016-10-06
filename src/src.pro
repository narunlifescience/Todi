#Todi MPD client music player
QT += widgets network xml

TARGET = Todi
TEMPLATE = app

DEPENDPATH += . gui lib
INCLUDEPATH += . lib
RESOURCES = ../data/icons.qrc

# if you would like qDebug() output please comment out yhe following line
DEFINES += QT_NO_DEBUG_OUTPUT

HEADERS += gui/AboutDialog.h \
           gui/Player.h \
           gui/IconLoader.h \
           gui/MpdConnectionDialog.h \
           gui/globals.h \
           widgets/TrackSlider.h \
           widgets/tracksliderpopup.h \
           lib/mpdclient.h \
           lib/commandcontroller.h \
           lib/mpdsocket.h \
           lib/playbackcontroller.h \
           lib/mpdmodel.h \
           lib/mpddata.h \
           lib/mpddataparser.h \
    lib/playbackoptionscontroller.h

FORMS +=   gui/AboutDialog.ui \
           gui/MpdConnectionDialog.ui \
           gui/Player.ui

SOURCES += main.cpp \
           gui/AboutDialog.cpp \
           gui/Player.cpp \
           gui/IconLoader.cpp \
           gui/Trayicon.cpp \
           gui/MpdConnectionDialog.cpp \
           gui/globals.cpp \
           widgets/TrackSlider.cpp \
           widgets/tracksliderpopup.cpp \
           lib/mpdclient.cpp \
           lib/commandcontroller.cpp \
           lib/mpdsocket.cpp \
           lib/playbackcontroller.cpp \
           lib/mpddata.cpp \
           lib/mpddataparser.cpp \
    lib/playbackoptionscontroller.cpp
