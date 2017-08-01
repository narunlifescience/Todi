#Todi MPD client music player
QT += widgets network xml svg

TARGET = Todi
TEMPLATE = app

DEPENDPATH += . gui lib
INCLUDEPATH += . lib
RESOURCES = ../data/icons.qrc

INCLUDEPATH += ../3rdparty/taglib/include
LIBS += -L"../3rdparty/taglib/lib" -ltag

# if you would like qDebug() output please comment out yhe following line
DEFINES += QT_NO_DEBUG_OUTPUT

HEADERS += core/application.h \
           core/lazy.h \
           gui/AboutDialog.h \
           gui/Player.h \
           gui/IconLoader.h \
           gui/MpdConnectionDialog.h \
           gui/globals.h \
           widgets/TrackSlider.h \
           widgets/tracksliderpopup.h \
           widgets/VolumePopup.h \
           lib/mpdclient.h \
           lib/mpdsocket.h \
           lib/playbackcontroller.h \
           lib/mpdmodel.h \
           lib/mpddata.h \
           lib/mpddataparser.h \
           lib/playbackoptionscontroller.h \
           tagger/currentartloader.h \
           widgets/currentsongmetadatalabel.h \
           widgets/currentcoverartlabel.h \
           lib/currentplaylistcontroller.h \
           gui/currentplaylistmodel.h \
           gui/currentplaylistview.h \
           gui/systemtrayicon.h

FORMS +=   gui/AboutDialog.ui \
           gui/MpdConnectionDialog.ui \
           gui/Player.ui

SOURCES += main.cpp \
           core/application.cpp \
           gui/AboutDialog.cpp \
           gui/Player.cpp \
           gui/IconLoader.cpp \
           gui/MpdConnectionDialog.cpp \
           gui/globals.cpp \
           widgets/TrackSlider.cpp \
           widgets/tracksliderpopup.cpp \
           widgets/VolumePopup.cpp \
           lib/mpdclient.cpp \
           lib/mpdsocket.cpp \
           lib/playbackcontroller.cpp \
           lib/mpddata.cpp \
           lib/mpddataparser.cpp \
           lib/playbackoptionscontroller.cpp \
           tagger/currentartloader.cpp \
           widgets/currentcoverartlabel.cpp \
           widgets/currentsongmetadatalabel.cpp \
           lib/currentplaylistcontroller.cpp \
           gui/currentplaylistmodel.cpp \
           gui/currentplaylistview.cpp \
           gui/systemtrayicon.cpp
