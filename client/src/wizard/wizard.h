#ifndef WIZARD_H
#define WIZARD_H

#include <QtWidgets/QWizardPage>

namespace Ui {
    class Wizard;
}

class Wizard : public QWizard {
    Q_OBJECT
public:
    Wizard(QWidget *parent = 0);
    ~Wizard();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Wizard *m_ui;

private slots:
    void on_Wizard_accepted();
};

#endif // WIZARD_H
