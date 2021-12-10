/****************************************************************************
** Meta object code from reading C++ file 'netbp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../netbp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'netbp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Network_t {
    QByteArrayData data[14];
    char stringdata0[134];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Network_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Network_t qt_meta_stringdata_Network = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Network"
QT_MOC_LITERAL(1, 8, 5), // "print"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 1), // "s"
QT_MOC_LITERAL(4, 17, 12), // "clearConsole"
QT_MOC_LITERAL(5, 30, 5), // "Clear"
QT_MOC_LITERAL(6, 36, 7), // "doLearn"
QT_MOC_LITERAL(7, 44, 9), // "stopLearn"
QT_MOC_LITERAL(8, 54, 16), // "BackPropLearning"
QT_MOC_LITERAL(9, 71, 21), // "vector<ExamplesData>&"
QT_MOC_LITERAL(10, 93, 4), // "data"
QT_MOC_LITERAL(11, 98, 8), // "maxEpoch"
QT_MOC_LITERAL(12, 107, 18), // "PerceptronLearning"
QT_MOC_LITERAL(13, 126, 7) // "setData"

    },
    "Network\0print\0\0s\0clearConsole\0Clear\0"
    "doLearn\0stopLearn\0BackPropLearning\0"
    "vector<ExamplesData>&\0data\0maxEpoch\0"
    "PerceptronLearning\0setData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Network[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    0,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   63,    2, 0x0a /* Public */,
       6,    0,   64,    2, 0x0a /* Public */,
       7,    0,   65,    2, 0x0a /* Public */,
       8,    2,   66,    2, 0x0a /* Public */,
      12,    2,   71,    2, 0x0a /* Public */,
      13,    2,   76,    2, 0x0a /* Public */,
      13,    1,   81,    2, 0x2a /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, QMetaType::Int,   10,   11,
    QMetaType::Void, 0x80000000 | 9, QMetaType::Int,   10,   11,
    QMetaType::Void, 0x80000000 | 9, QMetaType::Int,   10,   11,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void Network::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Network *_t = static_cast<Network *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->print((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->clearConsole(); break;
        case 2: _t->Clear(); break;
        case 3: _t->doLearn(); break;
        case 4: _t->stopLearn(); break;
        case 5: _t->BackPropLearning((*reinterpret_cast< vector<ExamplesData>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->PerceptronLearning((*reinterpret_cast< vector<ExamplesData>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->setData((*reinterpret_cast< vector<ExamplesData>(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->setData((*reinterpret_cast< vector<ExamplesData>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Network::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Network::print)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Network::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Network::clearConsole)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Network::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_Network.data,
    qt_meta_data_Network,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Network::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Network::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Network.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Network::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Network::print(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Network::clearConsole()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
