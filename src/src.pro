#Todi MPD client music player
QT += widgets network xml svg
CONFIG += c++11

TARGET = Todi
TEMPLATE = app

DEPENDPATH += . beautify gui lib
INCLUDEPATH += . lib beautify
RESOURCES = ../data/icons.qrc

INCLUDEPATH += ../3rdparty/taglib/include
LIBS += -L"../3rdparty/taglib/lib" -ltag

# if you would like qDebug() output please comment out the following line
DEFINES += QT_NO_DEBUG_OUTPUT

HEADERS += core/application.h \
           core/lazy.h \
           gui/AboutDialog.h \
           gui/Player.h \
           beautify/IconLoader.h \
           gui/MpdConnectionDialog.h \
           gui/globals.h \
           widgets/TrackSlider.h \
           widgets/tracksliderpopup.h \
           widgets/VolumePopup.h \
           lib/mpdclient.h \
           lib/mpdsocket.h \
           lib/playbackcontroller.h \
           lib/mpdmodel.h \
           lib/mpdfilemodel.h \
           lib/mpddata.h \
           lib/mpddataparser.h \
           lib/playbackoptionscontroller.h \
           tagger/currentartloader.h \
           widgets/currentsongmetadatalabel.h \
           widgets/currentcoverartlabel.h \
           lib/currentplaylistcontroller.h \
           gui/currentplaylistmodel.h \
           gui/currentplaylistview.h \
           gui/systemtrayicon.h \
           models/filemodel.h \
           widgets/tooltip/tips.h \
           widgets/tooltip/tooltip.h \
           widgets/tooltip/reuse.h \
           widgets/tooltip/effects.h \
           widgets/tooltip/faketooltip.h \
           utils/hostosinfo.h \
           utils/osspecificaspects.h \
           widgets/console/console.h \
           widgets/console/consolewidget.h \
           widgets/mainwidget.h \
           widgets/tabbar.h \
           widgets/stylehelper.h \
           widgets/qt_blurimage.h \
           widgets/metadatawidget.h \
    beautify/stylesheetproperties.h \
    beautify/theme.h \
    lib/mpdlibrarymodel.h \
    models/librarymodel.h

FORMS +=   gui/AboutDialog.ui \
           gui/MpdConnectionDialog.ui \
           gui/Player.ui \
           widgets/console/consolewidget.ui

SOURCES += main.cpp \
           core/application.cpp \
           gui/AboutDialog.cpp \
           gui/Player.cpp \
           beautify/IconLoader.cpp \
           gui/MpdConnectionDialog.cpp \
           gui/globals.cpp \
           models/filemodel.cpp \
           widgets/TrackSlider.cpp \
           widgets/tracksliderpopup.cpp \
           widgets/VolumePopup.cpp \
           lib/mpdclient.cpp \
           lib/mpdsocket.cpp \
           lib/playbackcontroller.cpp \
           lib/mpddata.cpp \
           lib/mpdfilemodel.cpp \
           lib/mpddataparser.cpp \
           lib/playbackoptionscontroller.cpp \
           tagger/currentartloader.cpp \
           widgets/currentcoverartlabel.cpp \
           widgets/currentsongmetadatalabel.cpp \
           lib/currentplaylistcontroller.cpp \
           gui/currentplaylistmodel.cpp \
           gui/currentplaylistview.cpp \
           gui/systemtrayicon.cpp \
           widgets/tooltip/tips.cpp \
           widgets/tooltip/tooltip.cpp \
           widgets/tooltip/faketooltip.cpp \
           widgets/console/console.cpp \
           widgets/console/consolewidget.cpp \
           utils/hostosinfo.cpp \
           widgets/mainwidget.cpp \
           widgets/tabbar.cpp \
           widgets/stylehelper.cpp \
           widgets/metadatawidget.cpp \
    beautify/theme.cpp \
    lib/mpdlibrarymodel.cpp \
    models/librarymodel.cpp
