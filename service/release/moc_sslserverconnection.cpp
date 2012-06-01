/****************************************************************************
** Meta object code from reading C++ file 'sslserverconnection.h'
**
** Created: Wed 11. Jan 09:03:50 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../sslserverconnection.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sslserverconnection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SslServerConnection[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   20,   20,   20, 0x0a,
      63,   20,   20,   20, 0x0a,
      79,   20,   20,   20, 0x0a,
     106,  102,   20,   20, 0x0a,
     151,  146,   20,   20, 0x0a,
     191,  188,   20,   20, 0x0a,
     215,  188,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SslServerConnection[] = {
    "SslServerConnection\0\0connectionIsClosed()\0"
    "slotAcceptedClient()\0slotStartRead()\0"
    "slotConnectionClosed()\0err\0"
    "slotError(QAbstractSocket::SocketError)\0"
    "mode\0slotModeChanged(QSslSocket::SslMode)\0"
    "id\0removeItemFromList(int)\0recDig(int)\0"
};

const QMetaObject SslServerConnection::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SslServerConnection,
      qt_meta_data_SslServerConnection, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SslServerConnection::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SslServerConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SslServerConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SslServerConnection))
        return static_cast<void*>(const_cast< SslServerConnection*>(this));
    return QThread::qt_metacast(_clname);
}

int SslServerConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: connectionIsClosed(); break;
        case 1: slotAcceptedClient(); break;
        case 2: slotStartRead(); break;
        case 3: slotConnectionClosed(); break;
        case 4: slotError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 5: slotModeChanged((*reinterpret_cast< QSslSocket::SslMode(*)>(_a[1]))); break;
        case 6: removeItemFromList((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: recDig((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void SslServerConnection::connectionIsClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
