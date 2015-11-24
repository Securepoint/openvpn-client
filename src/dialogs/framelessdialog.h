#ifndef FramelessDialog_h
#define FramelessDialog_h

#include <QDialog>

class FramelessDialog : public QDialog
{
    Q_OBJECT

public:
   FramelessDialog(QWidget * parent = 0);
   virtual ~FramelessDialog();
private:
    bool bIsFramelessSetup;
    QWidget * wrapper;
    QMetaObject::Connection connectionClose;
protected:
    void setupFrameless();
    void postSetupFrameless();

    QWidget * getWrapperWidget();

};

#endif // FramelessDialog_h