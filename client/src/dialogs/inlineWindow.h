#ifndef INLINEWINDOW_H
#define INLINEWINDOW_H


#include <QtGui>
#include <QShowEvent>

#include "FramelessDialog.h"
#include "widgets/settings/config/advconfigsettingswidget.h"

namespace Ui {
    class InlineWindow;
}

class InlineWindow : public FramelessDialog {
    Q_OBJECT
public:
    InlineWindow(QString tag, QString cert, QString* p_caValueInline, AdvConfigSettingsWidget* p_parent);
    ~InlineWindow();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    void showEvent (QShowEvent *e);
    void getCert(QString path);

private:
    Ui::InlineWindow *m_ui;
    QString tag;
    QString cert;
    QString* p_cert;
    AdvConfigSettingsWidget* p_parent;

private slots:
    void on_cmdClose_clicked();
    void on_cmdSave_clicked();


    signals:
    void sendSave();
};
#endif // INLINEWINDOW_H
