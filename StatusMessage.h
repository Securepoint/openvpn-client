#ifndef STATUSMESSAGE_H
#define STATUSMESSAGE_H

#include <QObject>
#include <QDialog>

QT_BEGIN_NAMESPACE
    class QLabel;
    class QLineEdit;
    class QTextEdit;
    class QPushButton;
QT_END_NAMESPACE

class StatusMessage : public QDialog
{
    Q_OBJECT

public:
    StatusMessage();
    QString messageText;
    QTextEdit *fileContent;
    QPushButton *cmdClose;
    QPushButton *cmdSave;
    QLabel *fPath;

 public slots:
    void closeDialog ();
    void saveContent ();
};

#endif // STATUSMESSAGE_H
