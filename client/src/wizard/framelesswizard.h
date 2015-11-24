#ifndef FrameLessWizard_h
#define FrameLessWizard_h

#include <QtWidgets/QWizard>
#include <QtWidgets/QFrame>
#include <QtWidgets/QPushButton>

class FramelessWizard : public QWizard
{
    Q_OBJECT

public:
   FramelessWizard(QWidget * parent = 0);
   virtual ~FramelessWizard();
private:
    bool bIsFramelessSetup;

    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;
    QFrame *line_4;
    QFrame *line_5;
    QPushButton * cmdClose;
protected:
    void setupFrameless();
    void resizeEvent(QResizeEvent * event);

private slots:
    void customCmdClose();
};


#endif // FrameLessWizard_h