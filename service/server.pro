TARGET = SPOpenVPNService
TEMPLATE = app
CONFIG += console \
    qt
QT = core \
    network
SOURCES = main.cpp \
    openvpn.cpp \
    sslserverconnection.cpp \
    sslserver.cpp \
    sslkeycontent.cpp \
    sslconnections.cpp \
    debug.cpp \
    service.cpp \
    srvcli.cpp \
    tapdriver.cpp
include(src/qtservice.pri)
HEADERS += openvpn.h \
    sslserverconnection.h \
    sslserver.h \
    sslkeycontent.h \
    sslconnections.h \
    paths.h \
    debug.h \
    service.h \
    qthreadexec.h \
    srvcli.h \
    tapdriver.h
FORMS += 



















