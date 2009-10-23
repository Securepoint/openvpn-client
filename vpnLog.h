#ifndef VPNLOG_H
#define VPNLOG_H

#include <QObject>
#include <QDialog>

QT_BEGIN_NAMESPACE
    class QLabel;
    class QLineEdit;
    class QTextEdit;
    class QPushButton;
QT_END_NAMESPACE

class VpnLog : public QDialog
{
    Q_OBJECT

public:
    VpnLog();
    QString messageText;
    QTextEdit *logContent;
    QPushButton *cmdClose;

 public slots:
    void closeDialog ();
};

#endif // VPNLOG_H
