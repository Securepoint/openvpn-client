#ifndef WIZ_CERTPAGE_H
#define WIZ_CERTPAGE_H

#include <QtGui/QWizardPage>
#include <QFileDialog>

namespace Ui {
    class CertPage;
}

class CertPage : public QWizardPage {
    Q_OBJECT
public:
    CertPage(QWidget *parent = 0);
    ~CertPage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CertPage *m_ui;
    QString lastDir;

private slots:
    void on_cmdOpenKey_clicked();
    void on_cmdOpenCert_clicked();
    void on_cmdOpenCA_clicked();

};

#endif // WIZ_CERTPAGE_H
