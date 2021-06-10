#ifndef CLIENTSETTINGSWIDGET_H
#define CLIENTSETTINGSWIDGET_H

#include <QtWidgets/QWidget>
#include <QtGui/QShowEvent>

namespace Ui {
    class ClientSettingsWidget;
}

class ClientSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientSettingsWidget(QWidget *parent = 0);
    ~ClientSettingsWidget();

protected:
    void paintEvent(QPaintEvent *pe);
    void showEvent(QShowEvent *event);

private slots:
    void on_cmdBack_clicked();

    void on_cbStartOnWindows_toggled(bool checked);

    void on_cbBlockShutdown_toggled(bool checked);

    void on_cbAlwaysPopup_toggled(bool checked);

    void on_cbInteract_toggled(bool checked);

    void on_cmdAddTap_clicked();

    void on_cmdRemoveTap_clicked();

    void on_rbUseManual_toggled(bool checked);

    void on_cbShowSmallDhKeyHInt_toggled(bool checked);

    void on_cbStartOnWindows_clicked(bool checked);

    void on_cbSendToTrayAfterConnect_toggled(bool checked);

public slots:
    void receivedTapCount(int count);


private:
    Ui::ClientSettingsWidget *ui;

    int tapCount;
};

#endif // CLIENTSETTINGSWIDGET_H
