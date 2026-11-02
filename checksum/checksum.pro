TEMPLATE = app
QT += core
TARGET = checksum

CONFIG += embed_manifest_exe
QMAKE_LFLAGS_WINDOWS += $$quote( /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\" )

SOURCES += main.cpp

DISTFILES += \
    checksum.rc

RC_FILE = checksum.rc
