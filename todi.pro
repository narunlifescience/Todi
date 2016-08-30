#-------------------------------------------------
#
#                   Todi client
#
#-------------------------------------------------

QT += widgets dbus
CONFIG += c++14

TARGET = Todi
TEMPLATE = app


SOURCES += \
    src/main.cpp\
    src/Player.cpp \
    src/IconLoader.cpp \
    src/MprisDbusHandler.cpp \
    src/MprisHandler.cpp

HEADERS  += \
    src/Player.h \
    src/IconLoader.h \
    src/MprisDbusHandler.h \
    src/MprisHandler.h

FORMS    += \
    src/Player.ui

RESOURCES += \
    data/icons.qrc
