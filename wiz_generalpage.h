#ifndef WIZ_GENERALPAGE_H
#define WIZ_GENERALPAGE_H

#include <QtGui/QWizardPage>

namespace Ui {
    class GeneralPage;
}

class GeneralPage : public QWizardPage {
    Q_OBJECT
public:
    GeneralPage(QWidget *parent = 0);
    ~GeneralPage();

protected:
    void changeEvent(QEvent *e);
    void initializePage();

private:
    Ui::GeneralPage *m_ui;
};

#endif // WIZ_GENERALPAGE_H
