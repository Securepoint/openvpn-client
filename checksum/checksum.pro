TEMPLATE = app
QT += core network
TARGET = checksum

CONFIG += embed_manifest_exe
QMAKE_LFLAGS_WINDOWS += $$quote( /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\" )

SOURCES += main.cpp \
    srvcli.cpp

HEADERS += \
    srvcli.h

DISTFILES += \
    checksum.rc

RC_FILE = checksum.rc
