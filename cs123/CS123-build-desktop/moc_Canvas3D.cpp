/****************************************************************************
** Meta object code from reading C++ file 'Canvas3D.h'
**
** Created: Sat Sep 29 10:30:29 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../projects/ui/Canvas3D.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Canvas3D.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Canvas3D[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_Canvas3D[] = {
    "Canvas3D\0"
};

const QMetaObject Canvas3D::staticMetaObject = {
    { &SupportCanvas3D::staticMetaObject, qt_meta_stringdata_Canvas3D,
      qt_meta_data_Canvas3D, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Canvas3D::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Canvas3D::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Canvas3D::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Canvas3D))
        return static_cast<void*>(const_cast< Canvas3D*>(this));
    return SupportCanvas3D::qt_metacast(_clname);
}

int Canvas3D::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SupportCanvas3D::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
