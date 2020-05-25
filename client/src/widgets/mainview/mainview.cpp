#include "mainview.h"

#include <Qdatastream.h>

#include <FrmMain.h>
#include <widgetfactory.h>
#include <config/Configs.h>
#include <service/SrvCLI.h>
#include <message.h>
#include <widgets/settings/config/advconfigsettingswidget.h>
#include <widgets/settings/config/quickeditwidget.h>

#include <dialogs/servicelog.h>

#include <widgets/settings/client/settings.h>


MainListView::MainListView(QWidget *parent)
    : QListView(parent)
{
    this->setMinimumHeight(100);

    //
    this->setItemDelegate(&this->delegate);
    // Set the model
    this->setModel(&this->model);

    this->viewport()->installEventFilter(this);
    this->setMouseTracking(true);
    //
    // Cursor handling
    QObject::connect(&this->delegate, SIGNAL(setCursor(Qt::CursorShape)), this, SLOT(setCursorSlot(Qt::CursorShape)));

    // Init connections menu
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));

    QObject::connect(&this->delegate, SIGNAL(updateIndex(QModelIndex)), this, SLOT(update(QModelIndex)));

    QObject::connect(&this->delegate, SIGNAL(buttonClicked(const QModelIndex&)), this, SLOT(connectionButtonClicked(const QModelIndex&)));

    QObject::connect(&this->delegate, SIGNAL(buttonDClick(const QModelIndex&)), this, SLOT(connectionButtonDClick(const QModelIndex&)));
}

bool MainListView::eventFilter(QObject *obj, QEvent *event)
{
    //
    // Check the events of the listview viewport
    // Whenn the cursor is over an invalid index or leaves the viewport
    // it is necessary to refresh the state of the buttons.
    //

    if (obj != this->viewport()) {
        return QWidget::eventFilter(obj, event);
    }

    switch (event->type()) {
        case QEvent::Leave: {
            this->delegate.clearLastState();
            break;
        }
        case QEvent::MouseMove: {
                QModelIndex index = this->indexAt(static_cast<QMouseEvent*>(event)->pos());
                if (!index.isValid()) {
                    this->delegate.clearLastState();
                }
                break;
        }
        default: {
            int fooDummy (0);
            Q_UNUSED(fooDummy)
            break;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MainListView::connectionButtonClicked(const QModelIndex& index)
{
    auto currentConnection =  this->model.GetConnection(index);

    switch (currentConnection->GetState()) {
        case ConnectionState::Connecting:
        case ConnectionState::Connected:
            currentConnection->Disconnect();
            currentConnection->SetState(ConnectionState::Disconnecting);
            currentConnection->SetLastUsed(QDateTime::currentDateTime().toTime_t());
            //
            break;
        default:
            // On all other states we do a connect
            currentConnection->SetState(ConnectionState::Connecting);

            // We need a callback if connection was successful
            if(!currentConnection->Connect()) {
                currentConnection->SetState(ConnectionState::Error);
            }
    }
}


void MainListView::connectionButtonDClick(const QModelIndex& index)
{
    auto pConnection =  this->model.GetConnection(index);

    ServiceLog logDialog (pConnection->GetId());
    logDialog.exec();
    // TODO: show error log
}

void MainListView::customContextMenuRequested(const QPoint &pos)
{
    // Check if the connections exits
    if(!this->model.GetConnection(this->currentIndex()))
        return;

    //// We only want the context menu, if the mouse is hovering an item
    //if((ListViewButtonDelegate::ButtonState)this->currentIndex().data(Qt::UserRole + 100).toInt() != ListViewButtonDelegate::Hover)
    //    return;


    QMenu menu;
    //
    menu.setCursor(Qt::PointingHandCursor);
    //
    menu.setObjectName("connectionActionsMenu");
    //

    QString connectText = QObject::tr("Connect");
    if(this->model.GetConnection(this->currentIndex())->GetState() == ConnectionState::Disconnected
         || this->model.GetConnection(this->currentIndex())->GetState() == ConnectionState::Error)
    {

    }
    else if(this->model.GetConnection(this->currentIndex())->GetState() == ConnectionState::Connecting)
    {
        connectText = QObject::tr("Cancel");
    }
    else {
        connectText = QObject::tr("Disconnect");
    }

    menu.addAction(QIcon(":/data/images/connect.png"), connectText, this, SLOT(ContextConnect()));

    if(Settings::instance()->managed()) {
        menu.addAction(QIcon(":/data/images/export.png"), QObject::tr("Export"), this, SLOT(ContextExport()));
        menu.addAction(QIcon(":/data/images/remove.png"), QObject::tr("Remove"), this, SLOT(ContextRemove()));
    }

    // TODO: change icon
    menu.addAction(QIcon(":/data/images/wheel-dark.png"), QObject::tr("Log"), this, SLOT(ContextLog()));

    if(Settings::instance()->managed())
        menu.addAction(QIcon(":/data/images/quick-edit.png"), QObject::tr("Quick Edit"), this, SLOT(openQuickEdit()));

    if(Settings::instance()->managed())
        menu.addAction(QIcon(":/data/images/wheel-dark.png"), QObject::tr("Settings"), this, SLOT(openConfigSettings()));

    menu.addSeparator();

    //
    menu.exec(this->mapToGlobal(pos));
}

void MainListView::setCursorSlot(Qt::CursorShape type)
{
    //
    // Set a new cursor
    //

    this->setCursor(type);
}

void MainListView::openQuickEdit()
{
    auto pConnection = ((MainListView*)FrmMain::instance()->mainWidget()->widget(MainView))->model.GetConnection(this->model.data(this->currentIndex(), Qt::UserRole + 8).toInt());
    QuickEditWidget widget(pConnection->GetConfigPath());

    widget.exec();
}

void MainListView::openConfigSettings()
{
   ((ConfigSettingsWidget*)FrmMain::instance()->mainWidget()->widget(ConfigSettings))->SetId(this->model.data(this->currentIndex(), Qt::UserRole + 8).toInt());
   FrmMain::instance()->mainWidget()->showWidget(ConfigSettings);
}



void MainListView::ContextConnect()
{
    this->connectionButtonClicked(this->currentIndex());
}

void MainListView::ContextExport()
{
   ((ExportWidget*)FrmMain::instance()->mainWidget()->widget(Export))->SetConfigPath(this->model.data(this->currentIndex(), Qt::UserRole + 7).toString());
   FrmMain::instance()->mainWidget()->showWidget(Export);
}

void MainListView::ContextRemove()
{
    if(Message::confirm(QObject::tr("Do you really want to delete this config?"), QObject::tr("Please confirm"), true)) {

        model.GetConnection(this->currentIndex())->Disconnect();

        model.GetConnection(this->currentIndex())->Delete();

        Configs::instance()->removeFromDatabase(model.GetConnection(this->currentIndex())->GetId());

        Configs::instance()->refreshConfigs();

        model.LoadConnections();

        for(const auto con : Configs::instance()->getList())
        {
            SrvCLI::instance()->send(QLatin1String("STATUS"), QString("%1").arg(con.second->GetId()));
        }

        FrmMain::instance()->refreshUI();
    }
}

void MainListView::ContextLog()
{
    this->connectionButtonDClick(this->currentIndex());
}
