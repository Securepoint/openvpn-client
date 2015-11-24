# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = SSLVpnClient
DESTDIR = ../Debug
QT += core sql network xml widgets gui
CONFIG += debug
CONFIG -= flat
DEFINES += _WINDOWS QT_WIDGETS_LIB QT_XML_LIB QT_NETWORK_LIB QT_SQL_LIB QT_OPENGL_ES_2_ANGLE NOMINMAX
INCLUDEPATH += . \
    ./src \
    ./debug \
    $(QTDIR)/mkspecs/win32-msvc2012 \
    ./GeneratedFiles \
    ./src/dialogs \
    ./src/widgets/settings/config \
    ./src/widgets/export
LIBS += -lgdi32 \
    -luser32
DEPENDPATH += .
MOC_DIR += debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(SSLVpnClient.pri)
TRANSLATIONS += sslvpnclient_de.ts