QT       += core gui gui-private waylandclient-private

TARGET = inputpanel-shell

TEMPLATE = lib
CONFIG += plugin


SOURCES += inputpanelshellplugin.cpp \
           qwaylandinputpanelshellintegration.cpp \
           qwaylandinputpanelsurface.cpp \
           qwayland-input-method-unstable-v1.cpp

HEADERS += qwaylandinputpanelsurface.h qwaylandinputpanelshellintegration.h qwayland-input-method-unstable-v1.h wayland-input-method-unstable-v1-client-protocol.h
OTHER_FILES += inputpanelshell.json

