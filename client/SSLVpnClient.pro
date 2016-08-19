TEMPLATE = app
TARGET = SSLVpnClient

QT += core sql network xml widgets gui

CONFIG += release

# Service
#DEFINES += _WINDOWS _WINDOWS QT_WIDGETS_LIB QT_XML_LIB QT_NETWORK_LIB QT_SQL_LIB QT_OPENGL_ES_2_ANGLE NOMINMAX QT_WIDGETS_LIB QT_XML_LIB QT_NETWORK_LIB QT_SQL_LIB QT_OPENGL_ES_2 QT_OPENGL_ES_2_ANGLE QT_DLL

# _PORTABLE
DEFINES += _PORTABLE PORTABLE _WINDOWS _WINDOWS QT_WIDGETS_LIB QT_XML_LIB QT_NETWORK_LIB QT_SQL_LIB QT_OPENGL_ES_2_ANGLE NOMINMAX QT_WIDGETS_LIB QT_XML_LIB QT_NETWORK_LIB QT_SQL_LIB QT_OPENGL_ES_2 QT_OPENGL_ES_2_ANGLE _PORTABLE QT_DLL

HEADERS += ./src/frmmain.h \
    ./src/listviewbuttondelegate.h \
    ./src/listviewsinglebuttondelegate.h \
    ./src/message.h \
    ./src/rightalignpushbuttonmenu.h \
    ./src/single_application.h \
    ./src/tapdriver.h \
    ./src/utils.h \
    ./src/widgetfactory.h \
    ./src/config/Configs.h \
    ./src/config/configvalues.h \
    ./src/database/crypt.h \
    ./src/database/database.h \
    ./src/dialogs/framelessdialog.h \
    ./src/dialogs/frmgetuserdata.h \
    ./src/dialogs/servicelog.h \
    ./src/service/Transaction.h \
    ./src/service/servicelogdata.h \
    ./src/service/srvcli.h \
    ./src/service/sslconnections.h \
    ./src/service/sslkeycontent.h \
    ./src/service/sslserver.h \
    ./src/service/sslserverconnection.h \
    ./src/service/tasktransaction.h \
    ./src/service/transactionthread.h \
    ./src/update/checkinfo.h \
    ./src/update/checkupdate.h \
    ./src/update/netmanager.h \
    ./src/update/parsexml.h \
    ./src/widgets/export/exportwidget.h \
    ./src/widgets/import/importwidget.h \
    ./src/widgets/info/informationwidget.h \
    ./src/widgets/mainview/connectiondata.h \
    ./src/widgets/mainview/connectiondelegate.h \
    ./src/widgets/mainview/connectionmodel.h \
    ./src/widgets/mainview/mainview.h \
    ./src/widgets/settings/client/clientsettingswidget.h \
    ./src/widgets/settings/client/settings.h \
    ./src/widgets/settings/config/advconfigsettingswidget.h \
    ./src/widgets/settings/config/configsettingswidget.h \
    ./src/widgets/settings/config/quickeditwidget.h \
    ./src/wizard/framelesswizard.h \
    ./src/wizard/wiz_advpage.h \
    ./src/wizard/wiz_certpage.h \
    ./src/wizard/wiz_endpage.h \
    ./src/wizard/wiz_generalpage.h \
    ./src/wizard/wiz_remotepage.h \
    ./src/wizard/wiz_startpage.h \
    ./src/wizard/wiz_vpnwizard.h \
    ./src/wizard/wizard.h \
    ./src/zip/czip.h \
    ./src/zip/unzip.h \
    ./src/zip/zip.h
SOURCES += ./src/frmmain.cpp \
    ./src/listviewbuttondelegate.cpp \
    ./src/listviewsinglebuttondelegate.cpp \
    ./src/main.cpp \
    ./src/message.cpp \
    ./src/rightalignpushbuttonmenu.cpp \
    ./src/single_application.cpp \
    ./src/tapdriver.cpp \
    ./src/utils.cpp \
    ./src/widgetfactory.cpp \
    ./src/config/Configs.cpp \
    ./src/config/configvalues.cpp \
    ./src/database/crypt.cpp \
    ./src/database/database.cpp \
    ./src/dialogs/framelessdialog.cpp \
    ./src/dialogs/frmgetuserdata.cpp \
    ./src/dialogs/servicelog.cpp \
    ./src/service/servicelogdata.cpp \
    ./src/service/srvcli.cpp \
    ./src/service/sslconnections.cpp \
    ./src/service/sslkeycontent.cpp \
    ./src/service/sslserver.cpp \
    ./src/service/sslserverconnection.cpp \
    ./src/service/tasktransaction.cpp \
    ./src/service/transactionthread.cpp \
    ./src/update/checkinfo.cpp \
    ./src/update/checkupdate.cpp \
    ./src/update/netmanager.cpp \
    ./src/update/parsexml.cpp \
    ./src/widgets/export/exportwidget.cpp \
    ./src/widgets/import/importwidget.cpp \
    ./src/widgets/info/informationwidget.cpp \
    ./src/widgets/mainview/connectiondata.cpp \
    ./src/widgets/mainview/connectiondelegate.cpp \
    ./src/widgets/mainview/connectionmodel.cpp \
    ./src/widgets/mainview/mainview.cpp \
    ./src/widgets/settings/client/clientsettingswidget.cpp \
    ./src/widgets/settings/client/settings.cpp \
    ./src/widgets/settings/config/advconfigsettingswidget.cpp \
    ./src/widgets/settings/config/configsettingswidget.cpp \
    ./src/widgets/settings/config/quickeditwidget.cpp \
    ./src/wizard/framelesswizard.cpp \
    ./src/wizard/wiz_advpage.cpp \
    ./src/wizard/wiz_certpage.cpp \
    ./src/wizard/wiz_endpage.cpp \
    ./src/wizard/wiz_generalpage.cpp \
    ./src/wizard/wiz_remotepage.cpp \
    ./src/wizard/wiz_startpage.cpp \
    ./src/wizard/wiz_vpnwizard.cpp \
    ./src/wizard/wizard.cpp \
    ./src/zip/czip.cpp \
    ./src/zip/unzip.cpp \
    ./src/zip/zip.cpp
FORMS += ./src/frmmain.ui \
    ./src/dialogs/frmgetuserdata.ui \
    ./src/dialogs/servicelog.ui \
    ./src/widgets/export/exportwidget.ui \
    ./src/widgets/import/importwidget.ui \
    ./src/widgets/info/informationwidget.ui \
    ./src/widgets/settings/client/clientsettingswidget.ui \
    ./src/widgets/settings/config/advconfigsettingswidget.ui \
    ./src/widgets/settings/config/configsettingswidget.ui \
    ./src/widgets/settings/config/quickeditwidget.ui \
    ./src/wizard/wiz_advpage.ui \
    ./src/wizard/wiz_certpage.ui \
    ./src/wizard/wiz_endpage.ui \
    ./src/wizard/wiz_generalpage.ui \
    ./src/wizard/wiz_remotepage.ui \
    ./src/wizard/wiz_startpage.ui \
    ./src/wizard/wizard.ui

INCLUDEPATH +=  . \
                src \
                src/config \
                src/database \
                src/dialogs \
                src/service \
                src/update \
                src/widgets \
                src/wizard \
                src/zip

LIBS += -llibEGL \
    -llibGLESv2 \
    -lgdi32 \
    -luser32

TRANSLATIONS += ./sslvpnclient_de.ts
RESOURCES += res.qrc

