/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../realtime-v2/src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    const uint offsetsAndSize[32];
    char stringdata0[239];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 10), // "MainWindow"
QT_MOC_LITERAL(11, 16), // "onPerPixelFilter"
QT_MOC_LITERAL(28, 0), // ""
QT_MOC_LITERAL(29, 19), // "onKernelBasedFilter"
QT_MOC_LITERAL(49, 12), // "onUploadFile"
QT_MOC_LITERAL(62, 13), // "onValChangeP1"
QT_MOC_LITERAL(76, 8), // "newValue"
QT_MOC_LITERAL(85, 13), // "onValChangeP2"
QT_MOC_LITERAL(99, 21), // "onValChangeNearSlider"
QT_MOC_LITERAL(121, 20), // "onValChangeFarSlider"
QT_MOC_LITERAL(142, 18), // "onValChangeNearBox"
QT_MOC_LITERAL(161, 17), // "onValChangeFarBox"
QT_MOC_LITERAL(179, 14), // "onExtraCredit1"
QT_MOC_LITERAL(194, 14), // "onExtraCredit2"
QT_MOC_LITERAL(209, 14), // "onExtraCredit3"
QT_MOC_LITERAL(224, 14) // "onExtraCredit4"

    },
    "MainWindow\0onPerPixelFilter\0\0"
    "onKernelBasedFilter\0onUploadFile\0"
    "onValChangeP1\0newValue\0onValChangeP2\0"
    "onValChangeNearSlider\0onValChangeFarSlider\0"
    "onValChangeNearBox\0onValChangeFarBox\0"
    "onExtraCredit1\0onExtraCredit2\0"
    "onExtraCredit3\0onExtraCredit4"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   92,    2, 0x08,    1 /* Private */,
       3,    0,   93,    2, 0x08,    2 /* Private */,
       4,    0,   94,    2, 0x08,    3 /* Private */,
       5,    1,   95,    2, 0x08,    4 /* Private */,
       7,    1,   98,    2, 0x08,    6 /* Private */,
       8,    1,  101,    2, 0x08,    8 /* Private */,
       9,    1,  104,    2, 0x08,   10 /* Private */,
      10,    1,  107,    2, 0x08,   12 /* Private */,
      11,    1,  110,    2, 0x08,   14 /* Private */,
      12,    0,  113,    2, 0x08,   16 /* Private */,
      13,    0,  114,    2, 0x08,   17 /* Private */,
      14,    0,  115,    2, 0x08,   18 /* Private */,
      15,    0,  116,    2, 0x08,   19 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onPerPixelFilter(); break;
        case 1: _t->onKernelBasedFilter(); break;
        case 2: _t->onUploadFile(); break;
        case 3: _t->onValChangeP1((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->onValChangeP2((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->onValChangeNearSlider((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->onValChangeFarSlider((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->onValChangeNearBox((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 8: _t->onValChangeFarBox((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 9: _t->onExtraCredit1(); break;
        case 10: _t->onExtraCredit2(); break;
        case 11: _t->onExtraCredit3(); break;
        case 12: _t->onExtraCredit4(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.offsetsAndSize,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MainWindow_t
, QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
