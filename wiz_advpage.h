#ifndef WIZ_ADVPAGE_H
#define WIZ_ADVPAGE_H

#include <QtGui/QWizardPage>

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

private:
    Ui::AdvPage *m_ui;
    void initializePage ();

private slots:
    void on_cmdInfoWinRouteMethod_clicked();
    void on_cmdInfoAuthUserPass_clicked();
};

#endif // WIZ_ADVPAGE_H
