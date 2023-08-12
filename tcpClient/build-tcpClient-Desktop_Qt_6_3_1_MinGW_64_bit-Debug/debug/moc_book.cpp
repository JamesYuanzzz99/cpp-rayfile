/****************************************************************************
** Meta object code from reading C++ file 'book.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../tcpClient/book.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'book.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Book_t {
    const uint offsetsAndSize[40];
    char stringdata0[176];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Book_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Book_t qt_meta_stringdata_Book = {
    {
QT_MOC_LITERAL(0, 4), // "Book"
QT_MOC_LITERAL(5, 9), // "createDir"
QT_MOC_LITERAL(15, 0), // ""
QT_MOC_LITERAL(16, 8), // "flushDir"
QT_MOC_LITERAL(25, 6), // "delDir"
QT_MOC_LITERAL(32, 9), // "renameDir"
QT_MOC_LITERAL(42, 8), // "enterDir"
QT_MOC_LITERAL(51, 11), // "QModelIndex"
QT_MOC_LITERAL(63, 5), // "index"
QT_MOC_LITERAL(69, 9), // "changeDir"
QT_MOC_LITERAL(79, 10), // "const PDU*"
QT_MOC_LITERAL(90, 3), // "pdu"
QT_MOC_LITERAL(94, 4), // "back"
QT_MOC_LITERAL(99, 10), // "uploadFile"
QT_MOC_LITERAL(110, 14), // "uploadFileData"
QT_MOC_LITERAL(125, 7), // "delFile"
QT_MOC_LITERAL(133, 12), // "downloadFile"
QT_MOC_LITERAL(146, 9), // "shareFile"
QT_MOC_LITERAL(156, 8), // "moveFile"
QT_MOC_LITERAL(165, 10) // "sltDestDir"

    },
    "Book\0createDir\0\0flushDir\0delDir\0"
    "renameDir\0enterDir\0QModelIndex\0index\0"
    "changeDir\0const PDU*\0pdu\0back\0uploadFile\0"
    "uploadFileData\0delFile\0downloadFile\0"
    "shareFile\0moveFile\0sltDestDir"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Book[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   98,    2, 0x0a,    1 /* Public */,
       3,    0,   99,    2, 0x0a,    2 /* Public */,
       4,    0,  100,    2, 0x0a,    3 /* Public */,
       5,    0,  101,    2, 0x0a,    4 /* Public */,
       6,    1,  102,    2, 0x0a,    5 /* Public */,
       9,    1,  105,    2, 0x0a,    7 /* Public */,
      12,    0,  108,    2, 0x0a,    9 /* Public */,
      13,    0,  109,    2, 0x0a,   10 /* Public */,
      14,    0,  110,    2, 0x0a,   11 /* Public */,
      15,    0,  111,    2, 0x0a,   12 /* Public */,
      16,    0,  112,    2, 0x0a,   13 /* Public */,
      17,    0,  113,    2, 0x0a,   14 /* Public */,
      18,    0,  114,    2, 0x0a,   15 /* Public */,
      19,    0,  115,    2, 0x0a,   16 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Book::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Book *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->createDir(); break;
        case 1: _t->flushDir(); break;
        case 2: _t->delDir(); break;
        case 3: _t->renameDir(); break;
        case 4: _t->enterDir((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 5: _t->changeDir((*reinterpret_cast< std::add_pointer_t<const PDU*>>(_a[1]))); break;
        case 6: _t->back(); break;
        case 7: _t->uploadFile(); break;
        case 8: _t->uploadFileData(); break;
        case 9: _t->delFile(); break;
        case 10: _t->downloadFile(); break;
        case 11: _t->shareFile(); break;
        case 12: _t->moveFile(); break;
        case 13: _t->sltDestDir(); break;
        default: ;
        }
    }
}

const QMetaObject Book::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Book.offsetsAndSize,
    qt_meta_data_Book,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Book_t
, QtPrivate::TypeAndForceComplete<Book, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const PDU *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Book::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Book::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Book.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Book::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
