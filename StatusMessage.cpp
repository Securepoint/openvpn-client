#include <QtGui>
#include "StatusMessage.h"

StatusMessage::StatusMessage()
{
    fileContent = new QTextEdit();
    fPath = new QLabel ();
    cmdClose = new QPushButton ("&Close", this);
    cmdSave = new QPushButton ("&Save", this);
    connect(cmdClose, SIGNAL(clicked()), this, SLOT(closeDialog()));
    connect(cmdSave, SIGNAL(clicked()), this, SLOT(saveContent()));

    QVBoxLayout *messageLayout = new QVBoxLayout;
        messageLayout->addWidget(fPath);
        messageLayout->addWidget(fileContent);
        messageLayout->addWidget(cmdSave);
        messageLayout->addWidget(cmdClose);
    setLayout(messageLayout);

    setWindowTitle(tr("Edit VPN Config"));
    setWindowFlags(Qt::Dialog);
    setModal (true);
    resize(500, 500);
}

void StatusMessage::closeDialog () {
    this->close();
}

void StatusMessage::saveContent() {
    // Speichern
    QString sPath = fPath->text();
    QFile saveFile (sPath);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
         return;

     QTextStream out(&saveFile);
     out << fileContent->toPlainText();
     saveFile.close();
     // Reload File
     QString fileReleoadContent = QString("");
     QFile cf (sPath);

     if (!cf.open(QIODevice::ReadOnly | QIODevice::Text))
          return;

     QTextStream in(&cf);
     while (!in.atEnd()) {
         QString line = in.readLine();
         fileReleoadContent += line + QString("\n");
     }
     cf.close();
     fileContent->setText(fileReleoadContent);
}


