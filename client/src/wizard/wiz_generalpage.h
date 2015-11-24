#ifndef WIZ_GENERALPAGE_H
#define WIZ_GENERALPAGE_H

#include <QtWidgets/QWizardPage>

namespace Ui {
    class GeneralPage;
}

class GeneralPage : public QWizardPage {
    Q_OBJECT
public:
    GeneralPage(QWidget *parent = 0);
    ~GeneralPage();

    virtual bool isComplete() const;

protected:
    void changeEvent(QEvent *e);
    void initializePage();
    

public slots:
    void inputTextChanged();

private:
    Ui::GeneralPage *m_ui;
};

#endif // WIZ_GENERALPAGE_H
