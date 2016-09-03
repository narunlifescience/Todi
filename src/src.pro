#Todi MPD client music player
QT += widgets network xml

TARGET = Todi
TEMPLATE = app

DEPENDPATH += . gui lib
INCLUDEPATH += . lib
RESOURCES = ../data/icons.qrc

# if you would like qDebug() output please comment out yhe following line
DEFINES += QT_NO_DEBUG_OUTPUT

HEADERS += gui/musiclibraryitem.h \
           gui/musiclibraryitemroot.h \
           gui/musiclibraryitemartist.h \
           gui/musiclibraryitemalbum.h \
           gui/musiclibraryitemsong.h \
           gui/musiclibrarymodel.h \
           gui/musiclibrarysortfiltermodel.h \
           gui/playlisttablemodel.h \
           gui/AboutDialog.h \
           gui/preferences_dialog.h \
           gui/statistics_dialog.h \
           gui/dirviewmodel.h \
           gui/dirviewproxymodel.h \
           gui/dirviewitem.h \
           gui/dirviewitemdir.h \
           gui/dirviewitemfile.h \
           gui/dirviewitemroot.h \
           gui/Player.h \
           gui/IconLoader.h \
           widgets/TrackSlider.h \
           gui/MpdConnectionDialog.h \
           gui/globals.h \
           widgets/tracksliderpopup.h \
           lib/mpdconnection.h \
           lib/mpddatabaseconnection.h \
           lib/mpdparseutils.h \
           lib/mpdstats.h \
           lib/mpdstatus.h \
           lib/song.h \

FORMS += gui/AboutDialog.ui \
         gui/preferences_dialog.ui \
         gui/statistics_dialog.ui \
         gui/MpdConnectionDialog.ui \
         gui/Player.ui

SOURCES += main.cpp \
           gui/musiclibraryitem.cpp \
           gui/musiclibraryitemroot.cpp \
           gui/musiclibraryitemartist.cpp \
           gui/musiclibraryitemalbum.cpp \
           gui/musiclibraryitemsong.cpp \
           gui/musiclibrarymodel.cpp \
           gui/musiclibrarysortfiltermodel.cpp \
           gui/playlisttablemodel.cpp \
           gui/AboutDialog.cpp \
           gui/preferences_dialog.cpp \
           gui/statistics_dialog.cpp \
           gui/dirviewmodel.cpp \
           gui/dirviewproxymodel.cpp \
           gui/dirviewitem.cpp \
           gui/dirviewitemfile.cpp \
           gui/dirviewitemdir.cpp \
           gui/dirviewitemroot.cpp \
           gui/Player.cpp \
           gui/IconLoader.cpp \
           widgets/TrackSlider.cpp \
           gui/MpdConnectionDialog.cpp \
           gui/globals.cpp \
           widgets/tracksliderpopup.cpp \
           lib/mpdconnection.cpp \
           lib/mpddatabaseconnection.cpp \
           lib/mpdparseutils.cpp \
           lib/mpdstats.cpp \
           lib/mpdstatus.cpp \
           lib/song.cpp \
