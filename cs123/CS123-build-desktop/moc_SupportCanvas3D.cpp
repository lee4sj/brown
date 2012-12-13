/****************************************************************************
** Meta object code from reading C++ file 'SupportCanvas3D.h'
**
** Created: Tue Dec 11 15:34:11 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../projects/ui/SupportCanvas3D.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SupportCanvas3D.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SupportCanvas3D[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   16,   16,   16, 0x0a,
      54,   16,   16,   16, 0x0a,
      71,   16,   16,   16, 0x0a,
      88,   16,   16,   16, 0x0a,
     105,   16,   16,   16, 0x0a,
     128,   16,   16,   16, 0x0a,
     154,   16,   16,   16, 0x0a,
     180,   16,   16,   16, 0x0a,
     204,   16,   16,   16, 0x0a,
     228,   16,   16,   16, 0x0a,
     252,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SupportCanvas3D[] = {
    "SupportCanvas3D\0\0aspectRatioChanged()\0"
    "resetUpVector()\0setCameraAxisX()\0"
    "setCameraAxisY()\0setCameraAxisZ()\0"
    "setCameraAxonometric()\0updateCameraHeightAngle()\0"
    "updateCameraTranslation()\0"
    "updateCameraRotationU()\0updateCameraRotationV()\0"
    "updateCameraRotationN()\0updateCameraClip()\0"
};

const QMetaObject SupportCanvas3D::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_SupportCanvas3D,
      qt_meta_data_SupportCanvas3D, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SupportCanvas3D::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SupportCanvas3D::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SupportCanvas3D::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SupportCanvas3D))
        return static_cast<void*>(const_cast< SupportCanvas3D*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int SupportCanvas3D::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: aspectRatioChanged(); break;
        case 1: resetUpVector(); break;
        case 2: setCameraAxisX(); break;
        case 3: setCameraAxisY(); break;
        case 4: setCameraAxisZ(); break;
        case 5: setCameraAxonometric(); break;
        case 6: updateCameraHeightAngle(); break;
        case 7: updateCameraTranslation(); break;
        case 8: updateCameraRotationU(); break;
        case 9: updateCameraRotationV(); break;
        case 10: updateCameraRotationN(); break;
        case 11: updateCameraClip(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void SupportCanvas3D::aspectRatioChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
