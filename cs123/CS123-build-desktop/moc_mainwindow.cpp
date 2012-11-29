/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Wed Nov 28 03:43:16 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../projects/ui/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      32,   11,   11,   11, 0x0a,
      50,   11,   11,   11, 0x0a,
      67,   11,   11,   11, 0x0a,
      77,   11,   11,   11, 0x0a,
      88,   11,   11,   11, 0x0a,
      99,   11,   11,   11, 0x0a,
     121,   11,   11,   11, 0x0a,
     145,   11,   11,   11, 0x0a,
     159,   11,   11,   11, 0x0a,
     173,   11,   11,   11, 0x0a,
     192,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0updateAspectRatio()\0"
    "settingsChanged()\0fileCopy3Dto2D()\0"
    "fileNew()\0fileOpen()\0fileSave()\0"
    "checkAllRayFeatures()\0uncheckAllRayFeatures()\0"
    "filterImage()\0renderImage()\0"
    "activateCanvas2D()\0activateCanvas3D()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateAspectRatio(); break;
        case 1: settingsChanged(); break;
        case 2: fileCopy3Dto2D(); break;
        case 3: fileNew(); break;
        case 4: fileOpen(); break;
        case 5: fileSave(); break;
        case 6: checkAllRayFeatures(); break;
        case 7: uncheckAllRayFeatures(); break;
        case 8: filterImage(); break;
        case 9: renderImage(); break;
        case 10: activateCanvas2D(); break;
        case 11: activateCanvas3D(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
