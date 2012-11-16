/****************************************************************************
** Meta object code from reading C++ file 'canvas.h'
**
** Created: Fri Oct 12 15:31:32 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../lab04/src/canvas.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'canvas.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Lab5Canvas[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,   37,   11,   11, 0x09,
      72,   11,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_Lab5Canvas[] = {
    "Lab5Canvas\0\0_mouseDrag(QMouseEvent*)\0"
    "event\0mouseDragEvent(QMouseEvent*)\0"
    "redraw()\0"
};

const QMetaObject Lab5Canvas::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_Lab5Canvas,
      qt_meta_data_Lab5Canvas, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Lab5Canvas::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Lab5Canvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Lab5Canvas::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Lab5Canvas))
        return static_cast<void*>(const_cast< Lab5Canvas*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int Lab5Canvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _mouseDrag((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 1: mouseDragEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: redraw(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Lab5Canvas::_mouseDrag(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
