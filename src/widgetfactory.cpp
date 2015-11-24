#include "widgetfactory.h"
#include <widgets/settings/client/settings.h>

WidgetFactory::WidgetFactory(QWidget *parent)
    : QWidget(parent),
      view (0),
      info(0),
      import (0),
      clientSettings(0),
      currentWidget(0),
      configSettings(0),
      _export(0)
{
    // Set the vertical layout as main layout
    this->setLayout(&this->mainLayout);
    // Init the layout
    this->mainLayout.setContentsMargins(0, 0, 0, 0);
    this->mainLayout.setSpacing(0);
    //
    this->mainLayout.addWidget(this->widget(MainView));

    this->currentWidget = this->widget(MainView);
}

WidgetFactory::~WidgetFactory()
{
    //
    // Cleanup
    //

    delete this->view;
    this->view = 0;

    delete this->info;
    this->info = 0;

    delete this->import;
    this->import = 0;

    delete this->clientSettings;
    this->clientSettings = 0;

    delete this->configSettings;
    this->configSettings = 0;

    delete this->_export;
    this->_export = 0;
}

void WidgetFactory::showWidget(int type)
{
    //
    // Change the shown widget
    //
    //
    this->currentWidget->hide();
    //
    this->mainLayout.removeWidget(this->currentWidget);
    //
    auto widget = this->widget(type);

    this->mainLayout.addWidget(widget);
    //
    this->currentWidget = widget;

    this->currentWidget->show();
}

void WidgetFactory::showInformation()
{
    //
    // Show the information widget
    //

    this->showWidget(Information);
}

void WidgetFactory::showImport()
{
    //
    // Show the import widget
    //

    this->showWidget(Import);
}

void WidgetFactory::showClientSettings()
{
    //
    // Show the client settings widget
    //

    // Refresh setting data
    Settings::instance()->loadOrRefresh();
    //
    this->showWidget(ClientSettings);
}

QWidget *WidgetFactory::widget(int type)
{
    //
    // Returns a new widget
    //

    switch (type) {
        case Information:
            if (!this->info) {
                this->info = new InformationWidget;
            }

            return this->info;
        case Import:
            if (!this->import) {
                this->import = new ImportWidget;
            }

            return this->import;
        case ClientSettings:
            if (!this->clientSettings) {
                this->clientSettings = new ClientSettingsWidget;
            }

            return this->clientSettings;
        case ConfigSettings:
            if(!this->configSettings) {
                this->configSettings = new ConfigSettingsWidget;
            }

            return this->configSettings;
        case Export:
            if(!this->_export) {
                this->_export = new ExportWidget;
            }

            return this->_export;
        case MainView:
        default:
            if (!this->view) {
                this->view = new MainListView;
            }

            return this->view;
    }

    // That never should happen ;p
    return 0;
}
