#ifndef WIZ_ADVPAGE_H
#define WIZ_ADVPAGE_H

#include <QtWidgets/QWizardPage>

namespace Ui {
    class AdvPage;
}

class AdvPage : public QWizardPage {
    Q_OBJECT
public:
    AdvPage(QWidget *parent = 0);
    ~AdvPage();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_cbWinDirUseDefault_toggled(bool checked);

    void on_cbWinDirOther_toggled(bool checked);

    void on_cbWinDirEnvironment_toggled(bool checked);

    void on_cbWinDirPath_toggled(bool checked);

private:
    Ui::AdvPage *m_ui;
    void initializePage ();

};

#endif // WIZ_ADVPAGE_H
