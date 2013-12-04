#include "checkinfo.h"

CheckInfo::CheckInfo() :
    build (""),
    version (0),
    major (-1),
    minor (-1)
{
}

// Setter
void CheckInfo::setBuild (const QString &buildStr)
{
    this->build = buildStr;
    if (buildStr.contains(".")) {
        QStringList resList = buildStr.split(".");
        if (resList.size() > 3) {
            this->version = resList.at(1).toUInt();
            this->major = resList.at(2).toInt();
            this->minor = resList.at(3).toInt();
        } else
            if (resList.size() == 3 ) {
                this->version = resList.at(1).toUInt();
                this->major = resList.at(2).toInt();
                this->minor = -1;
            } else
                if (resList.size() == 2 ) {
                    this->version = resList.at(1).toUInt();
                    this->major = -1;
                    this->minor = -1;
                }
    }
}

// Add
void CheckInfo::addBugFix (const QString &bugFix)
{
    this->bugFixes.append(bugFix);
}
void CheckInfo::addFeature (const QString &feature)
{
    this->features.append(feature);
}
void CheckInfo::addComment (const QString &comment)
{
    this->comments.append(comment);
}


// Getter
QString CheckInfo::getBuild() const
{
    return this->build;
}

quint32 CheckInfo::getVersion() const
{
    return this->version;
}

qint32 CheckInfo::getMajor() const
{
    return this->major;
}

qint32 CheckInfo::getMinor() const
{
    return this->minor;
}

QStringList CheckInfo::getBugFixes() const
{
    return this->bugFixes;
}

QStringList CheckInfo::getFeatures() const
{
    return this->features;
}

QStringList CheckInfo::getComments() const
{
    return this->comments;
}

void CheckInfo::clear()
{
    this->build.clear();
    this->version = 0;
    this->major = -1;
    this->minor = -1;
    this->bugFixes.clear();
    this->features.clear();
    this->comments.clear();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CheckInfoList *CheckInfoList::mInst = NULL;

CheckInfoList *CheckInfoList::getInstance()
{
    if (mInst == NULL) mInst = new CheckInfoList ();
    return mInst;
}

CheckInfoList::CheckInfoList()
{


}


// Add

void CheckInfoList::addCheckInfo (const CheckInfo &chkInfo)
{
    this->list.append(chkInfo);
}

// Getter

QList <CheckInfo> CheckInfoList::getCheckInfoList() const
{
    return this->list;
}

qint32 CheckInfoList::getSizeofList() const
{
    return this->list.size();
}

// Clearer

void CheckInfoList::clearCheckInfoList()
{
    this->list.clear();
}
