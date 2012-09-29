/****************************************************************************
** Meta object code from reading C++ file 'SupportCanvas2D.h'
**
** Created: Sat Sep 29 10:30:27 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../projects/ui/SupportCanvas2D.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SupportCanvas2D.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SupportCanvas2D[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   16,   17,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SupportCanvas2D[] = {
    "SupportCanvas2D\0\0bool\0revertImage()\0"
};

const QMetaObject SupportCanvas2D::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SupportCanvas2D,
      qt_meta_data_SupportCanvas2D, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SupportCanvas2D::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SupportCanvas2D::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SupportCanvas2D::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SupportCanvas2D))
        return static_cast<void*>(const_cast< SupportCanvas2D*>(this));
    return QWidget::qt_metacast(_clname);
}

int SupportCanvas2D::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = revertImage();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
