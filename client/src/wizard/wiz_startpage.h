#ifndef WIZ_STARTPAGE_H
#define WIZ_STARTPAGE_H

#include <QtWidgets/QWizardPage>

namespace Ui {
    class CStartPage;
}

class CStartPage : public QWizardPage {
    Q_OBJECT
public:
    CStartPage(QWidget *parent = 0);
    ~CStartPage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CStartPage *m_ui;
};

#endif // WIZ_STARTPAGE_H
