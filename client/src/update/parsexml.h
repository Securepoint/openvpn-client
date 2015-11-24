#ifndef PARSEXML_H
#define PARSEXML_H

#include <QtCore>
#include <QtXml/QDomDocument>
//
#include "checkupdate.h"
#include "checkinfo.h"

class ParseXML : public QObject
{
    Q_OBJECT
public:
    ParseXML();
    ~ParseXML();
    void runParser();
    QString highestVersion ();

private:
    QDomDocument doc;
    CheckUpdate chk;
    CheckInfo chkInfo;
    void traverseNode(const QDomNode &node);

private slots:
    void parseXmlString(const QString &xmlStr);
    void receivedError (const QString errMessage);

signals:
    void finished(bool success, QString errMessage = QString(""));
};

#endif // PARSEXML_H
