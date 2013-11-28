#ifndef CHECKINFO_H
#define CHECKINFO_H
#include <QList>
#include <QStringList>

class CheckInfo
{
public:
    CheckInfo();
    // Getter
    QString getBuild() const;
    quint32 getVersion() const;
    qint32 getMajor() const;
    qint32 getMinor() const;
    QStringList getBugFixes() const;
    QStringList getFeatures() const;
    QStringList getComments() const;
    // Setter
    void setBuild (const QString &buildStr);
    // Add
    void addBugFix (const QString &bugFix);
    void addFeature (const QString &feature);
    void addComment (const QString &comment);
    // clearer
    void clear();

private:
    QString build; // String: 10.6.2
    quint32 version; // 10
    quint32 major;   // 6
    quint32 minor;    // 2
    QStringList bugFixes;
    QStringList features;
    QStringList comments;
};

class CheckInfoList
{
public:
    static CheckInfoList *getInstance();
    // Add
    void addCheckInfo (const CheckInfo &chkInfo);
    // Getter
    QList <CheckInfo> getCheckInfoList() const;
    qint32 getSizeofList() const;
    //
    void clearCheckInfoList();

private:
    CheckInfoList();
    static CheckInfoList *mInst;
    QList <CheckInfo> list;
};

#endif // CHECKINFO_H
