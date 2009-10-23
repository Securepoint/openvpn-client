HEADERS = window.h \
    Configs.h \
    StatusMessage.h \
    userauth.h \
    openvpn.h \
    vpnLog.h \
    preferences.h \
    openvpnqlistitem.h \
    appfunc.h \
    wiz_startpage.h \
    wiz_generalpage.h \
    wiz_remotepage.h \
    wiz_endpage.h \
    wiz_vpnwizard.h \
    wiz_certpage.h \
    wiz_advpage.h
SOURCES = main.cpp \
    window.cpp \
    Configs.cpp \
    StatusMessage.cpp \
    userauth.cpp \
    openvpn.cpp \
    vpnLog.cpp \
    preferences.cpp \
    openvpnqlistitem.cpp \
    appfunc.cpp \
    wiz_startpage.cpp \
    wiz_generalpage.cpp \
    wiz_remotepage.cpp \
    wiz_endpage.cpp \
    wiz_vpnwizard.cpp \
    wiz_certpage.cpp \
    wiz_advpage.cpp
RESOURCES = systray.qrc
QT += xml \
    svg

# install
target.path = $$[QT_INSTALL_EXAMPLES]/desktop/systray
sources.files = $$SOURCES \
    $$HEADERS \
    $$RESOURCES \
    $$FORMS \
    systray.pro \
    resources \
    images
sources.path = $$[QT_INSTALL_EXAMPLES]/desktop/systray
INSTALLS += target \
    sources
wince* { 
    CONFIG(debug, release|debug):addPlugins.sources = $$QT_BUILD_TREE/plugins/imageformats/qsvgd4.dll
    CONFIG(release, release|debug):addPlugins.sources = $$QT_BUILD_TREE/plugins/imageformats/qsvg4.dll
    addPlugins.path = imageformats
    DEPLOYMENT += addPlugins
}
FORMS += preferences.ui \
    wiz_startpage.ui \
    wiz_generalpage.ui \
    wiz_remotepage.ui \
    wiz_endpage.ui \
    wiz_certpage.ui \
    wiz_advpage.ui
