HEADERS = Configs.h \
    openvpn.h \
    preferences.h \
    openvpnqlistitem.h \
    appfunc.h \
    appinfo.h \
    single_application.h \
    vpnlog.h \
    tapdriver.h \
    datacontroll.h \
    servicelog.h \
    treebutton.h \
    treeconitem.h \
    frmgetuserdata.h \
    mainwindowcontroll.h \
    settings.h \
    crypt.h \
    usercontroll.h \
    proxysettings.h \
    wiz_startpage.h \
    wiz_remotepage.h \
    wiz_generalpage.h \
    wiz_endpage.h \
    wiz_certpage.h \
    wiz_advpage.h \
    wiz_vpnwizard.h \
    deleteconfig.h \
    renameconfig.h \
    editconfig.h \
    configexport.h \
    importconfig.h \
    manageconnection.h \
    check64.h
SOURCES = main.cpp \
    Configs.cpp \
    openvpn.cpp \
    preferences.cpp \
    openvpnqlistitem.cpp \
    appfunc.cpp \
    appinfo.cpp \
    tapdriver.cpp \
    single_application.cpp \
    vpnlog.cpp \
    datacontroll.cpp \
    servicelog.cpp \
    treeconitem.cpp \
    treebutton.cpp \
    frmgetuserdata.cpp \
    mainwindowcontroll.cpp \
    settings.cpp \
    crypt.cpp \
    usercontroll.cpp \
    proxysettings.cpp \
    wiz_vpnwizard.cpp \
    wiz_startpage.cpp \
    wiz_remotepage.cpp \
    wiz_generalpage.cpp \
    wiz_endpage.cpp \
    wiz_certpage.cpp \
    wiz_advpage.cpp \
    deleteconfig.cpp \
    renameconfig.cpp \
    editconfig.cpp \
    configexport.cpp \
    importconfig.cpp \
    manageconnection.cpp \
    check64.cpp
RESOURCES = systray.qrc
QT += xml \
    svg \
    network

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
    appinfo.ui \
    vpnlog.ui \
    servicelog.ui \
    frmgetuserdata.ui \
    proxysettings.ui \
    wiz_startpage.ui \
    wiz_remotepage.ui \
    wiz_generalpage.ui \
    wiz_endpage.ui \
    wiz_certpage.ui \
    wiz_advpage.ui \
    deleteconfig.ui \
    renameconfig.ui \
    editconfig.ui \
    configexport.ui \
    importconfig.ui \
    manageconnection.ui
RC_FILE = myapp.rc
TRANSLATIONS = SpSSLVpn_ger.ts
