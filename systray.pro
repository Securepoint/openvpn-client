TARGET = Spvpncl

HEADERS = configs.h \
    openvpn.h \
    preferences.h \
    openvpnqlistitem.h \
    appfunc.h \
    appinfo.h \
    single_application.h \
    vpnlog.h \
    tapdriver.h \
    servicelog.h \
    treebutton.h \
    treeconitem.h \
    frmgetuserdata.h \
    settings.h \
    proxysettings.h \
    wizard/wiz_startpage.h \
    wizard/wiz_remotepage.h \
    wizard/wiz_generalpage.h \
    wizard/wiz_endpage.h \
    wizard/wiz_certpage.h \
    wizard/wiz_advpage.h \
    wizard/wiz_vpnwizard.h \
    deleteconfig.h \
    renameconfig.h \
    editconfig.h \
    configexport.h \
    importconfig.h \
    manageconnection.h \
    check64.h \
    message.h \
    debug.h \
    servicelogdata.h \
    network/transactionthread.h \
    network/Transaction.h \
    network/tasktransaction.h \
    network/sslserverconnection.h \
    network/sslserver.h \
    network/sslkeycontent.h \
    network/sslconnections.h \
    network/srvcli.h \
    network/paths.h \
    qthreadexec.h \
    zip.h \
    crypt.h \
    frmsaveddata.h \
    database.h \
    update/parsexml.h \
    update/checkupdate.h \
    update/checkinfo.h \
    frmupdatesettings.h \
    netmanager.h
SOURCES = main.cpp \
    configs.cpp \
    openvpn.cpp \
    preferences.cpp \
    openvpnqlistitem.cpp \
    appfunc.cpp \
    appinfo.cpp \
    tapdriver.cpp \
    single_application.cpp \
    vpnlog.cpp \
    servicelog.cpp \
    treeconitem.cpp \
    treebutton.cpp \
    frmgetuserdata.cpp \
    settings.cpp \
    proxysettings.cpp \
    wizard/wiz_vpnwizard.cpp \
    wizard/wiz_startpage.cpp \
    wizard/wiz_remotepage.cpp \
    wizard/wiz_generalpage.cpp \
    wizard/wiz_endpage.cpp \
    wizard/wiz_certpage.cpp \
    wizard/wiz_advpage.cpp \
    deleteconfig.cpp \
    renameconfig.cpp \
    editconfig.cpp \
    configexport.cpp \
    importconfig.cpp \
    manageconnection.cpp \
    check64.cpp \
    message.cpp \
    debug.cpp \
    servicelogdata.cpp \
    network/transactionthread.cpp \
    network/tasktransaction.cpp \
    network/sslserverconnection.cpp \
    network/sslserver.cpp \
    network/sslkeycontent.cpp \
    network/sslconnections.cpp \
    network/srvcli.cpp \
    zip.cpp \
    crypt.cpp \
    frmsaveddata.cpp \
    database.cpp \
    update/parsexml.cpp \
    update/checkupdate.cpp \
    update/checkinfo.cpp \
    frmupdatesettings.cpp \
    netmanager.cpp

win32::INCLUDEPATH += "quazip/zlib-1.2.5" \
                      "qca/include/QtCrypto"

win32::LIBS += "quazip/libquazip.a" \
               "qca/lib/libqca2.a" \

include("qca/crypto.prf")

RESOURCES = systray.qrc
QT += xml svg network sql

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

FORMS += preferences.ui \
    appinfo.ui \
    vpnlog.ui \
    servicelog.ui \
    frmgetuserdata.ui \
    proxysettings.ui \
    wizard/wiz_startpage.ui \
    wizard/wiz_remotepage.ui \
    wizard/wiz_generalpage.ui \
    wizard/wiz_endpage.ui \
    wizard/wiz_certpage.ui \
    wizard/wiz_advpage.ui \
    deleteconfig.ui \
    renameconfig.ui \
    editconfig.ui \
    configexport.ui \
    importconfig.ui \
    manageconnection.ui \
    frmsaveddata.ui \
    frmupdatesettings.ui

RC_FILE = myapp.rc
TRANSLATIONS = SpSSLVpn_ger.ts


































