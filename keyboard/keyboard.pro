#-------------------------------------------------
#
# Project created by QtCreator 2019-12-13T12:00:00
#
#-------------------------------------------------

QT += core gui quickwidgets qml waylandclient-private

TARGET = exor_keyboard
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        exordebug.cpp \
        exorkeyboardsettings.cpp \
        exorkeyboardpanel.cpp \
        exorkeyboardwayland.cpp \
        input-method-unstable-v1_p.c \
        EPADProxy.cpp \
        main.cpp \
        nfcthread.cpp \
        epaddbusclient.cpp

HEADERS += \
        exordebug.h \
        exorkeyboardsettings.h \
        exorkeyboardpanel.h \
        exorkeyboardwayland.h \
        input-method-unstable-v1.h \
        text-input-unstable-v1.h \
        EPADProxy.h \
        nfcthread.h \
        epaddbusclient.h


RESOURCES += qml.qrc

CONFIG += link_pkgconfig
PKGCONFIG += weston
