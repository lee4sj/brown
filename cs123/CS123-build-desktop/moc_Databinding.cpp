/****************************************************************************
** Meta object code from reading C++ file 'Databinding.h'
**
** Created: Wed Nov 28 03:43:16 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../projects/ui/Databinding.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Databinding.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DataBinding[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_DataBinding[] = {
    "DataBinding\0\0dataChanged()\0"
};

const QMetaObject DataBinding::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DataBinding,
      qt_meta_data_DataBinding, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DataBinding::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DataBinding::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DataBinding::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataBinding))
        return static_cast<void*>(const_cast< DataBinding*>(this));
    return QObject::qt_metacast(_clname);
}

int DataBinding::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: dataChanged(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void DataBinding::dataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_IntBinding[] = {

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
      21,   12,   11,   11, 0x05,
      36,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      58,   12,   11,   11, 0x08,
      74,   12,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_IntBinding[] = {
    "IntBinding\0\0newValue\0updateInt(int)\0"
    "updateString(QString)\0intChanged(int)\0"
    "stringChanged(QString)\0"
};

const QMetaObject IntBinding::staticMetaObject = {
    { &DataBinding::staticMetaObject, qt_meta_stringdata_IntBinding,
      qt_meta_data_IntBinding, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &IntBinding::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *IntBinding::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *IntBinding::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IntBinding))
        return static_cast<void*>(const_cast< IntBinding*>(this));
    return DataBinding::qt_metacast(_clname);
}

int IntBinding::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DataBinding::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateInt((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: updateString((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: intChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: stringChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void IntBinding::updateInt(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IntBinding::updateString(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_FloatBinding[] = {

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
      23,   14,   13,   13, 0x05,
      38,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      60,   14,   13,   13, 0x08,
      76,   14,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FloatBinding[] = {
    "FloatBinding\0\0newValue\0updateInt(int)\0"
    "updateString(QString)\0intChanged(int)\0"
    "stringChanged(QString)\0"
};

const QMetaObject FloatBinding::staticMetaObject = {
    { &DataBinding::staticMetaObject, qt_meta_stringdata_FloatBinding,
      qt_meta_data_FloatBinding, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FloatBinding::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FloatBinding::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FloatBinding::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FloatBinding))
        return static_cast<void*>(const_cast< FloatBinding*>(this));
    return DataBinding::qt_metacast(_clname);
}

int FloatBinding::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DataBinding::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateInt((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: updateString((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: intChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: stringChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void FloatBinding::updateInt(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FloatBinding::updateString(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_BoolBinding[] = {

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
      22,   13,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BoolBinding[] = {
    "BoolBinding\0\0newValue\0boolChanged(bool)\0"
};

const QMetaObject BoolBinding::staticMetaObject = {
    { &DataBinding::staticMetaObject, qt_meta_stringdata_BoolBinding,
      qt_meta_data_BoolBinding, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BoolBinding::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BoolBinding::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BoolBinding::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BoolBinding))
        return static_cast<void*>(const_cast< BoolBinding*>(this));
    return DataBinding::qt_metacast(_clname);
}

int BoolBinding::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DataBinding::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: boolChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_ChoiceBinding[] = {

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
      24,   15,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ChoiceBinding[] = {
    "ChoiceBinding\0\0newValue\0intChanged(int)\0"
};

const QMetaObject ChoiceBinding::staticMetaObject = {
    { &DataBinding::staticMetaObject, qt_meta_stringdata_ChoiceBinding,
      qt_meta_data_ChoiceBinding, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ChoiceBinding::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ChoiceBinding::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ChoiceBinding::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChoiceBinding))
        return static_cast<void*>(const_cast< ChoiceBinding*>(this));
    return DataBinding::qt_metacast(_clname);
}

int ChoiceBinding::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DataBinding::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: intChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
