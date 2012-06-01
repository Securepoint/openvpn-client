#ifndef WIZ_ENDPAGE_H
#define WIZ_ENDPAGE_H

#include <QtGui/QWizardPage>

namespace Ui {
    class EndPage;
}

class EndPage : public QWizardPage {
    Q_OBJECT
public:
    EndPage(QWidget *parent = 0);
    ~EndPage();

protected:
    void changeEvent(QEvent *e);
    void initializePage ();

private:
    Ui::EndPage *m_ui;
};

#endif // WIZ_ENDPAGE_H
