#ifndef WIZ_ENDPAGE_H
#define WIZ_ENDPAGE_H

#include <QtWidgets/QWizardPage>

namespace Ui {
    class CEndPage;
}

class CEndPage : public QWizardPage {
    Q_OBJECT
public:
    CEndPage(QWidget *parent = 0);
    ~CEndPage();

protected:
    void changeEvent(QEvent *e);
    void initializePage ();

private:
    Ui::CEndPage *m_ui;
};

#endif // WIZ_ENDPAGE_H
