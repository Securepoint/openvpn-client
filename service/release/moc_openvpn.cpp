/****************************************************************************
** Meta object code from reading C++ file 'openvpn.h'
**
** Created: Wed 11. Jan 11:34:50 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../openvpn.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'openvpn.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OpenVpn[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,    9,    8,    8, 0x05,
      36,    9,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      54,   48,    8,    8, 0x0a,
      65,    8,    8,    8, 0x0a,
      87,   81,    8,    8, 0x08,
     128,    8,    8,    8, 0x08,
     166,  146,    8,    8, 0x08,
     212,  208,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_OpenVpn[] = {
    "OpenVpn\0\0id\0removeItemFromList(int)\0"
    "foobar(int)\0ident\0setId(int)\0"
    "disconnectVpn()\0error\0"
    "showProcessError(QProcess::ProcessError)\0"
    "readProcessData()\0exitCode,exitStatus\0"
    "processFinished(int,QProcess::ExitStatus)\0"
    "err\0errorSocket(QAbstractSocket::SocketError)\0"
};

const QMetaObject OpenVpn::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_OpenVpn,
      qt_meta_data_OpenVpn, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OpenVpn::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OpenVpn::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OpenVpn::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OpenVpn))
        return static_cast<void*>(const_cast< OpenVpn*>(this));
    return QObject::qt_metacast(_clname);
}

int OpenVpn::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: removeItemFromList((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: foobar((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: setId((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 3: disconnectVpn(); break;
        case 4: showProcessError((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        case 5: readProcessData(); break;
        case 6: processFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 7: errorSocket((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void OpenVpn::removeItemFromList(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OpenVpn::foobar(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
