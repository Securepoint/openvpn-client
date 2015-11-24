#ifndef WIDGETFACTORY_H
#define WIDGETFACTORY_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QListView>

#include "widgets/info/informationwidget.h"
#include "widgets/mainview/mainview.h"
#include "widgets/import/importwidget.h"
#include "widgets/settings/client/clientsettingswidget.h"
#include "widgets\export\exportwidget.h"
#include "widgets\settings\config\configsettingswidget.h"

enum WidgetTypes
{
    MainView,
    Information,
    Import,
    ClientSettings,
    ConfigSettings,
    Export,
};

class WidgetFactory : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetFactory(QWidget *parent = 0);
    virtual ~WidgetFactory();
    void showWidget (int type);

public slots:
    void showInformation ();
    void showImport ();
    void showClientSettings ();
    QWidget *widget (int type);
private:
    QVBoxLayout mainLayout;

    MainListView *view;
    InformationWidget *info;
    ImportWidget *import;
    ClientSettingsWidget *clientSettings;
    ConfigSettingsWidget *configSettings;
    ExportWidget *_export;

    QWidget *currentWidget;

    
};

#endif // WIDGETFACTORY_H
