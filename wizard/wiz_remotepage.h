#ifndef WIZ_REMOTEPAGE_H
#define WIZ_REMOTEPAGE_H

#include <QtGui/QWizardPage>

namespace Ui {
    class RemotePage;
}

class RemotePage : public QWizardPage {
    Q_OBJECT
public:
    RemotePage(QWidget *parent = 0);
    ~RemotePage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::RemotePage *m_ui;
    void initializePage ();

};

#endif // WIZ_REMOTEPAGE_H
