/****************************************************************************
** Meta object code from reading C++ file 'userlogin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../userlogin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userlogin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UserLogin_t {
    QByteArrayData data[19];
    char stringdata0[324];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UserLogin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UserLogin_t qt_meta_stringdata_UserLogin = {
    {
QT_MOC_LITERAL(0, 0, 9), // "UserLogin"
QT_MOC_LITERAL(1, 10, 23), // "sendloginSigToMapwindow"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 8), // "userName"
QT_MOC_LITERAL(4, 44, 23), // "getVerificationCode_sig"
QT_MOC_LITERAL(5, 68, 16), // "registerUser_sig"
QT_MOC_LITERAL(6, 85, 8), // "password"
QT_MOC_LITERAL(7, 94, 7), // "verCode"
QT_MOC_LITERAL(8, 102, 21), // "loginToBikeClient_sig"
QT_MOC_LITERAL(9, 124, 19), // "connectToServer_sig"
QT_MOC_LITERAL(10, 144, 21), // "reconnectToServer_sig"
QT_MOC_LITERAL(11, 166, 24), // "getVerificationCode_slot"
QT_MOC_LITERAL(12, 191, 17), // "registerUser_slot"
QT_MOC_LITERAL(13, 209, 22), // "loginToBikeClient_slot"
QT_MOC_LITERAL(14, 232, 20), // "connectToServer_slot"
QT_MOC_LITERAL(15, 253, 22), // "reconnectToServer_slot"
QT_MOC_LITERAL(16, 276, 8), // "ReadData"
QT_MOC_LITERAL(17, 285, 9), // "ReadError"
QT_MOC_LITERAL(18, 295, 28) // "QAbstractSocket::SocketError"

    },
    "UserLogin\0sendloginSigToMapwindow\0\0"
    "userName\0getVerificationCode_sig\0"
    "registerUser_sig\0password\0verCode\0"
    "loginToBikeClient_sig\0connectToServer_sig\0"
    "reconnectToServer_sig\0getVerificationCode_slot\0"
    "registerUser_slot\0loginToBikeClient_slot\0"
    "connectToServer_slot\0reconnectToServer_slot\0"
    "ReadData\0ReadError\0QAbstractSocket::SocketError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UserLogin[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,
       5,    3,   85,    2, 0x06 /* Public */,
       8,    2,   92,    2, 0x06 /* Public */,
       9,    0,   97,    2, 0x06 /* Public */,
      10,    0,   98,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   99,    2, 0x08 /* Private */,
      12,    3,  102,    2, 0x08 /* Private */,
      13,    2,  109,    2, 0x08 /* Private */,
      14,    0,  114,    2, 0x08 /* Private */,
      15,    0,  115,    2, 0x08 /* Private */,
      16,    0,  116,    2, 0x08 /* Private */,
      17,    1,  117,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    6,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,    2,

       0        // eod
};

void UserLogin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UserLogin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendloginSigToMapwindow((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->getVerificationCode_sig((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->registerUser_sig((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 3: _t->loginToBikeClient_sig((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->connectToServer_sig(); break;
        case 5: _t->reconnectToServer_sig(); break;
        case 6: _t->getVerificationCode_slot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->registerUser_slot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 8: _t->loginToBikeClient_slot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: _t->connectToServer_slot(); break;
        case 10: _t->reconnectToServer_slot(); break;
        case 11: _t->ReadData(); break;
        case 12: _t->ReadError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 12:
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
            using _t = void (UserLogin::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserLogin::sendloginSigToMapwindow)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UserLogin::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserLogin::getVerificationCode_sig)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UserLogin::*)(const QString & , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserLogin::registerUser_sig)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (UserLogin::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserLogin::loginToBikeClient_sig)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (UserLogin::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserLogin::connectToServer_sig)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (UserLogin::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserLogin::reconnectToServer_sig)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UserLogin::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_UserLogin.data,
    qt_meta_data_UserLogin,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UserLogin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserLogin::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UserLogin.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UserLogin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void UserLogin::sendloginSigToMapwindow(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UserLogin::getVerificationCode_sig(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UserLogin::registerUser_sig(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void UserLogin::loginToBikeClient_sig(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void UserLogin::connectToServer_sig()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void UserLogin::reconnectToServer_sig()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
