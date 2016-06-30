TEMPLATE = app
TARGET = SPSSLVpnService

QT += core sql network

CONFIG += release console

DEFINES += _CONSOLE QT_DLL QT_HAVE_MMX QT_HAVE_3DNOW QT_HAVE_SSE QT_HAVE_MMXEXT QT_HAVE_SSE2 QT_XML_LIB QT_NETWORK_LIB QT_SQL_LIB

HEADERS += ./debug.h \
    ./qthreadexec.h \
    ./sslconnections.h \
    ./openvpn.h \
    ./service.h \
    ./srvcli.h \
    ./sslserver.h \
    ./sslserverconnection.h \
    ./tapdriver.h \
    ./src/qtservice.h \
    ./src/qtservice_p.h
SOURCES += ./debug.cpp \
    ./main.cpp \
    ./openvpn.cpp \
    ./service.cpp \
    ./srvcli.cpp \
    ./sslconnections.cpp \
    ./sslserver.cpp \
    ./sslserverconnection.cpp \
    ./tapdriver.cpp \
    ./src/qtservice.cpp \
    ./src/qtservice_win.cpp


LIBS += -lgdi32 \
    -luser32

