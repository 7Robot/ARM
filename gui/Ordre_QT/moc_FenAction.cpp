/****************************************************************************
** Meta object code from reading C++ file 'FenAction.h'
**
** Created: Fri Feb 24 08:38:44 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "FenAction.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FenAction.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FenAction[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      22,   10,   10,   10, 0x08,
      35,   10,   10,   10, 0x08,
      48,   10,   10,   10, 0x08,
      62,   10,   10,   10, 0x08,
      83,   77,   10,   10, 0x08,
     103,   10,   10,   10, 0x08,
     118,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FenAction[] = {
    "FenAction\0\0rs232_up()\0rs232_down()\0"
    "rs232_left()\0rs232_right()\0rs232_center()\0"
    "state\0rs232_checkbox(int)\0rs232_slider()\0"
    "rs232_envoi()\0"
};

const QMetaObject FenAction::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FenAction,
      qt_meta_data_FenAction, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FenAction::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FenAction::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FenAction::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FenAction))
        return static_cast<void*>(const_cast< FenAction*>(this));
    return QWidget::qt_metacast(_clname);
}

int FenAction::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: rs232_up(); break;
        case 1: rs232_down(); break;
        case 2: rs232_left(); break;
        case 3: rs232_right(); break;
        case 4: rs232_center(); break;
        case 5: rs232_checkbox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: rs232_slider(); break;
        case 7: rs232_envoi(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
