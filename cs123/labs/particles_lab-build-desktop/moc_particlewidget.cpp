/****************************************************************************
** Meta object code from reading C++ file 'particlewidget.h'
**
** Created: Tue Nov 27 19:33:02 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../lab06/src/particlewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'particlewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ParticleWidget[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      26,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      57,   51,   15,   15, 0x09,
      86,   15,   15,   15, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_ParticleWidget[] = {
    "ParticleWidget\0\0_glInit()\0"
    "_mouseDrag(QMouseEvent*)\0event\0"
    "mouseDragEvent(QMouseEvent*)\0redraw()\0"
};

const QMetaObject ParticleWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_ParticleWidget,
      qt_meta_data_ParticleWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ParticleWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ParticleWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ParticleWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ParticleWidget))
        return static_cast<void*>(const_cast< ParticleWidget*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int ParticleWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _glInit(); break;
        case 1: _mouseDrag((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: mouseDragEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: redraw(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ParticleWidget::_glInit()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ParticleWidget::_mouseDrag(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
