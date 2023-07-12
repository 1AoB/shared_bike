/****************************************************************************
** Meta object code from reading C++ file 'networkinterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../networkinterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkinterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_networkInterface_t {
    QByteArrayData data[34];
    char stringdata0[498];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_networkInterface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_networkInterface_t qt_meta_stringdata_networkInterface = {
    {
QT_MOC_LITERAL(0, 0, 16), // "networkInterface"
QT_MOC_LITERAL(1, 17, 23), // "ServerConnectSuccessSig"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 21), // "ServerConnectFalseSig"
QT_MOC_LITERAL(4, 64, 3), // "msg"
QT_MOC_LITERAL(5, 68, 20), // "msgTipPrintToHtmlSig"
QT_MOC_LITERAL(6, 89, 6), // "tipMsg"
QT_MOC_LITERAL(7, 96, 13), // "addBikeResSig"
QT_MOC_LITERAL(8, 110, 6), // "bikeId"
QT_MOC_LITERAL(9, 117, 9), // "longitude"
QT_MOC_LITERAL(10, 127, 8), // "latitude"
QT_MOC_LITERAL(11, 136, 16), // "deleteBikeResSig"
QT_MOC_LITERAL(12, 153, 19), // "sendBikeStartResSig"
QT_MOC_LITERAL(13, 173, 17), // "sendBikeEndResSig"
QT_MOC_LITERAL(14, 191, 23), // "bikeInfoBroadcastResSig"
QT_MOC_LITERAL(15, 215, 6), // "bikeid"
QT_MOC_LITERAL(16, 222, 6), // "status"
QT_MOC_LITERAL(17, 229, 17), // "ListAllBikeMsgSig"
QT_MOC_LITERAL(18, 247, 9), // "isSuccess"
QT_MOC_LITERAL(19, 257, 7), // "success"
QT_MOC_LITERAL(20, 265, 13), // "scanQRRequest"
QT_MOC_LITERAL(21, 279, 7), // "eventID"
QT_MOC_LITERAL(22, 287, 6), // "bikeID"
QT_MOC_LITERAL(23, 294, 9), // "ReadError"
QT_MOC_LITERAL(24, 304, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(25, 333, 13), // "getPermission"
QT_MOC_LITERAL(26, 347, 17), // "connectBikeServer"
QT_MOC_LITERAL(27, 365, 22), // "connectBikeServer_test"
QT_MOC_LITERAL(28, 388, 15), // "quiteBikeClient"
QT_MOC_LITERAL(29, 404, 26), // "sendRequestToGetAllBikeMsg"
QT_MOC_LITERAL(30, 431, 11), // "addBikeSlot"
QT_MOC_LITERAL(31, 443, 14), // "deleteBikeSlot"
QT_MOC_LITERAL(32, 458, 18), // "openScanCodeWindow"
QT_MOC_LITERAL(33, 477, 20) // "sendBroadcastRequest"

    },
    "networkInterface\0ServerConnectSuccessSig\0"
    "\0ServerConnectFalseSig\0msg\0"
    "msgTipPrintToHtmlSig\0tipMsg\0addBikeResSig\0"
    "bikeId\0longitude\0latitude\0deleteBikeResSig\0"
    "sendBikeStartResSig\0sendBikeEndResSig\0"
    "bikeInfoBroadcastResSig\0bikeid\0status\0"
    "ListAllBikeMsgSig\0isSuccess\0success\0"
    "scanQRRequest\0eventID\0bikeID\0ReadError\0"
    "QAbstractSocket::SocketError\0getPermission\0"
    "connectBikeServer\0connectBikeServer_test\0"
    "quiteBikeClient\0sendRequestToGetAllBikeMsg\0"
    "addBikeSlot\0deleteBikeSlot\0"
    "openScanCodeWindow\0sendBroadcastRequest"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_networkInterface[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  119,    2, 0x06 /* Public */,
       3,    1,  120,    2, 0x06 /* Public */,
       5,    1,  123,    2, 0x06 /* Public */,
       7,    3,  126,    2, 0x06 /* Public */,
      11,    1,  133,    2, 0x06 /* Public */,
      12,    0,  136,    2, 0x06 /* Public */,
      13,    0,  137,    2, 0x06 /* Public */,
      14,    4,  138,    2, 0x06 /* Public */,
      17,    4,  147,    2, 0x06 /* Public */,
      18,    1,  156,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      20,    4,  159,    2, 0x0a /* Public */,
      23,    1,  168,    2, 0x0a /* Public */,
      25,    0,  171,    2, 0x0a /* Public */,
      26,    0,  172,    2, 0x0a /* Public */,
      27,    0,  173,    2, 0x0a /* Public */,
      28,    0,  174,    2, 0x0a /* Public */,
      29,    0,  175,    2, 0x0a /* Public */,
      30,    3,  176,    2, 0x0a /* Public */,
      31,    1,  183,    2, 0x0a /* Public */,
      32,    3,  186,    2, 0x0a /* Public */,
      33,    4,  193,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Double, QMetaType::Double,    8,    9,   10,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double, QMetaType::Double,   15,   16,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double, QMetaType::Double,    8,   16,    9,   10,
    QMetaType::Void, QMetaType::Bool,   19,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double, QMetaType::Double,   21,   22,    9,   10,
    QMetaType::Void, 0x80000000 | 24,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Double, QMetaType::Double,    8,    9,   10,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Double, QMetaType::Double,    8,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double, QMetaType::Double,    8,   16,    9,   10,

       0        // eod
};

void networkInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<networkInterface *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ServerConnectSuccessSig(); break;
        case 1: _t->ServerConnectFalseSig((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->msgTipPrintToHtmlSig((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->addBikeResSig((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const double(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3]))); break;
        case 4: _t->deleteBikeResSig((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 5: _t->sendBikeStartResSig(); break;
        case 6: _t->sendBikeEndResSig(); break;
        case 7: _t->bikeInfoBroadcastResSig((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3])),(*reinterpret_cast< const double(*)>(_a[4]))); break;
        case 8: _t->ListAllBikeMsgSig((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3])),(*reinterpret_cast< const double(*)>(_a[4]))); break;
        case 9: _t->isSuccess((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->scanQRRequest((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 11: _t->ReadError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 12: _t->getPermission(); break;
        case 13: _t->connectBikeServer(); break;
        case 14: _t->connectBikeServer_test(); break;
        case 15: _t->quiteBikeClient(); break;
        case 16: _t->sendRequestToGetAllBikeMsg(); break;
        case 17: _t->addBikeSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 18: _t->deleteBikeSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->openScanCodeWindow((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 20: _t->sendBroadcastRequest((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (networkInterface::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&networkInterface::ServerConnectSuccessSig)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (networkInterface::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&networkInterface::ServerConnectFalseSig)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (networkInterface::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&networkInterface::msgTipPrintToHtmlSig)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (networkInterface::*)(const int , const double , const double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&networkInterface::addBikeResSig)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (networkInterface::*)(const int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&networkInterface::deleteBikeResSig)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (networkInterface::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&networkInterface::sendBikeStartResSig)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (networkInterface::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&networkInterface::sendBikeEndResSig)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (networkInterface::*)(const int , const int , const double , const double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&networkInterface::bikeInfoBroadcastResSig)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (networkInterface::*)(const int , const int , const double , const double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&networkInterface::ListAllBikeMsgSig)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (networkInterface::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&networkInterface::isSuccess)) {
                *result = 9;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject networkInterface::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_networkInterface.data,
    qt_meta_data_networkInterface,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *networkInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *networkInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_networkInterface.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int networkInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void networkInterface::ServerConnectSuccessSig()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void networkInterface::ServerConnectFalseSig(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void networkInterface::msgTipPrintToHtmlSig(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void networkInterface::addBikeResSig(const int _t1, const double _t2, const double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void networkInterface::deleteBikeResSig(const int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void networkInterface::sendBikeStartResSig()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void networkInterface::sendBikeEndResSig()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void networkInterface::bikeInfoBroadcastResSig(const int _t1, const int _t2, const double _t3, const double _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void networkInterface::ListAllBikeMsgSig(const int _t1, const int _t2, const double _t3, const double _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void networkInterface::isSuccess(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
