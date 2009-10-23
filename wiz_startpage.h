#ifndef WIZ_STARTPAGE_H
#define WIZ_STARTPAGE_H

#include <QtGui/QWizardPage>

namespace Ui {
    class StartPage;
}

class StartPage : public QWizardPage {
    Q_OBJECT
public:
    StartPage(QWidget *parent = 0);
    ~StartPage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::StartPage *m_ui;
};

#endif // WIZ_STARTPAGE_H
