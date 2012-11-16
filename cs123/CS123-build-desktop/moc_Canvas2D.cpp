/****************************************************************************
** Meta object code from reading C++ file 'Canvas2D.h'
**
** Created: Fri Nov 16 10:33:31 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../projects/ui/Canvas2D.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Canvas2D.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Canvas2D[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      25,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Canvas2D[] = {
    "Canvas2D\0\0cancelRender()\0filterImage()\0"
};

const QMetaObject Canvas2D::staticMetaObject = {
    { &SupportCanvas2D::staticMetaObject, qt_meta_stringdata_Canvas2D,
      qt_meta_data_Canvas2D, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Canvas2D::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Canvas2D::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Canvas2D::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Canvas2D))
        return static_cast<void*>(const_cast< Canvas2D*>(this));
    return SupportCanvas2D::qt_metacast(_clname);
}

int Canvas2D::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SupportCanvas2D::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: cancelRender(); break;
        case 1: filterImage(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
