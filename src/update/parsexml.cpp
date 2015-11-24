#include "parsexml.h"

ParseXML::ParseXML()
{
    QObject::connect(&this->chk, SIGNAL(finished(QString)), this, SLOT(parseXmlString(QString)));
    QObject::connect(&this->chk, SIGNAL(error(QString)), this, SLOT(receivedError(QString)));
}

ParseXML::~ParseXML()
{
}

void ParseXML::runParser()
{
    this->chk.sendRequest();
}

void ParseXML::parseXmlString(const QString &xmlStr)
{
    this->doc.setContent(xmlStr);
    // docElement now refers to the node "xml"
    QDomElement docElem = this->doc.documentElement();
    traverseNode(docElem);
    //
    emit this->finished(true);
}

void ParseXML::traverseNode(const QDomNode &node)
{
    static qint32 i(0);
    QDomNode domNode = node.firstChild();
    while(!domNode.isNull()) {
        if(domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()) {
                if (domElement.tagName() == QLatin1String("changelog")) {
                    this->chkInfo.clear();
                    this->chkInfo.setBuild(domElement.attribute("build"));
                    i++;
                    if (i > 0) {
                        CheckInfoList::getInstance()->addCheckInfo(this->chkInfo);
                    }
                }
            }
        }
        domNode = domNode.nextSibling();
    }
}

QString ParseXML::highestVersion()
{
    if (CheckInfoList::getInstance()->getSizeofList() <= 0) {
        return QObject::tr("n/a");
    }

    return CheckInfoList::getInstance()->getCheckInfoList().at(0).getBuild();
}

void ParseXML::receivedError(const QString errMessage)
{
    emit finished(false, errMessage);
}
