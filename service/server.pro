TARGET = SPOpenVPNService
TEMPLATE = app
CONFIG += console \
    qt
QT = core \
    network
SOURCES = main.cpp \
    openvpn.cpp
include(src/qtservice.pri)
HEADERS += openvpn.h
FORMS += 
