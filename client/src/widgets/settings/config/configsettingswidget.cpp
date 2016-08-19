#include "configsettingswidget.h"

#include <frmmain.h>
#include <widgetfactory.h>
#include <QtGui/qpainter.h>
#include <config/ConfigValues.h>
#include <config/configs.h>
#include "advconfigsettingswidget.h"
#include <FrmMain.h>
#include <utils.h>
#include <Database/Database.h>
#include <Database/Crypt.h>
#include <message.h>
#include <widgetfactory.h>

ConfigSettingsWidget::ConfigSettingsWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

ConfigSettingsWidget::~ConfigSettingsWidget()
{

}


void ConfigSettingsWidget::on_cmdBack_clicked()
{
    auto pConnection = ((MainListView*)FrmMain::instance()->mainWidget()->widget(MainView))->model.GetConnection(id);

    pConnection->SetAutoStart(this->ui.cbAutoStart->isChecked());

    Configs::instance()->refreshConfigs();

    ((MainListView*)FrmMain::instance()->mainWidget()->widget(MainView))->model.LoadConnections();

    FrmMain::instance()->mainWidget()->showWidget(MainView);
}

void ConfigSettingsWidget::on_cmdAdv_clicked()
{
    auto pConnection = ((MainListView*)FrmMain::instance()->mainWidget()->widget(MainView))->model.GetConnection(id);
    AdvConfigSettingsWidget widget(pConnection, FrmMain::instance());
    widget.setWindowFlags(widget.windowFlags() | Qt::Dialog);
    widget.exec();
}

void ConfigSettingsWidget::paintEvent(QPaintEvent *pe)
{
    //
    // Draw it with stylesheet support
    // Sometimes I hate Qt!
    // For further information, take a look at
    // http://stackoverflow.com/questions/18344135/why-do-stylesheets-not-work-when-subclassing-qwidget-and-using-q-object
    //

    Q_UNUSED(pe)

    QStyleOption option;
    option.initFrom(this);
    //
    QPainter painter(this);
    //
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}

void ConfigSettingsWidget::SetId(int id)
{
    this->id = id;

    auto pConnection = ((MainListView*)FrmMain::instance()->mainWidget()->widget(MainView))->model.GetConnection(id);

    ui.txtName->setText(pConnection->GetName());

    ui.cbAutoStart->setChecked(pConnection->IsAutoStart());
}

void ConfigSettingsWidget::on_cmdRemoveCreds_clicked()
{
    auto pConnection = ((MainListView*)FrmMain::instance()->mainWidget()->widget(MainView))->model.GetConnection(id);

    for(int i = 0; i < 7; ++i)
    {
        pConnection->saveUserData(id, i, "", true);
    }

    Message::information(QObject::tr("User data has been deleted successfully"), QObject::tr("User Information"), FrmMain::instance());
}

void ConfigSettingsWidget::on_cmdSetName_clicked()
{
     auto pConnection = ((MainListView*)FrmMain::instance()->mainWidget()->widget(MainView))->model.GetConnection(id);

     auto configDir = Utils::userApplicationDataDirectory() + "/config";

        //
	QDir dir;
    dir.rename(configDir + QLatin1String ("/") + pConnection->GetName(), configDir + QLatin1String ("/") + ui.txtName->text());

	QFile file;
    file.rename(configDir + QLatin1String ("/") + ui.txtName->text() + ("/") + pConnection->GetName() + (".ovpn"), configDir + QLatin1String ("/") + ui.txtName->text() + ("/") + ui.txtName->text() + (".ovpn"));

    QString sql (QString("UPDATE vpn SET \"vpn-name\" = '%1' WHERE \"vpn-id\" = %2")
                .arg(Crypt::encodePlaintext(Database::instance()->makeCleanValue(ui.txtName->text())))
                 .arg(id));

    Database::instance()->execute(sql);

    QString sql2 (QString("UPDATE vpn SET \"vpn-config\" = '%1' WHERE \"vpn-id\" = %2")
		.arg(Crypt::encodePlaintext(Database::instance()->makeCleanValue(configDir + QLatin1String ("/") + ui.txtName->text() + ("/") + ui.txtName->text() + (".ovpn"))))
		.arg(id));

    Database::instance()->execute(sql2);

    pConnection->SetName(ui.txtName->text());
    pConnection->SetConfigPath(configDir + QLatin1String ("/") + ui.txtName->text() + ("/") + ui.txtName->text() + (".ovpn"));
}