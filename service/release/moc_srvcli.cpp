/****************************************************************************
** Meta object code from reading C++ file 'srvcli.h'
**
** Created: Thu 5. Jan 12:56:26 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../srvcli.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'srvcli.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SrvCLI[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      32,    8,    7,    7, 0x0a,
      74,   59,    7,    7, 0x2a,
     104,   96,    7,    7, 0x2a,
     118,   59,    7,    7, 0x08,
     147,   96,    7,    7, 0x28,
     168,    7,    7,    7, 0x08,
     182,    7,    7,    7, 0x08,
     209,    7,    7,    7, 0x08,
     223,  217,    7,    7, 0x08,
     262,  255,    7,    7, 0x08,
     301,  295,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SrvCLI[] = {
    "SrvCLI\0\0command,params,fastmode\0"
    "send(QString,QString,bool)\0command,params\0"
    "send(QString,QString)\0command\0"
    "send(QString)\0sendRequest(QString,QString)\0"
    "sendRequest(QString)\0stopRequest()\0"
    "connectionClosedByServer()\0error()\0"
    "error\0slot_peerVerifyError(QSslError)\0"
    "errors\0slot_sslErrors(QList<QSslError>)\0"
    "state\0slot_stateChanged(QAbstractSocket::SocketState)\0"
};

const QMetaObject SrvCLI::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SrvCLI,
      qt_meta_data_SrvCLI, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SrvCLI::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SrvCLI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SrvCLI::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SrvCLI))
        return static_cast<void*>(const_cast< SrvCLI*>(this));
    return QObject::qt_metacast(_clname);
}

int SrvCLI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: send((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: send((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: send((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: sendRequest((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: sendRequest((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: stopRequest(); break;
        case 6: connectionClosedByServer(); break;
        case 7: error(); break;
        case 8: slot_peerVerifyError((*reinterpret_cast< const QSslError(*)>(_a[1]))); break;
        case 9: slot_sslErrors((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        case 10: slot_stateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
