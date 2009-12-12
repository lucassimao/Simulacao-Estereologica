/****************************************************************************
** Meta object code from reading C++ file 'DialogParametrosCubo.h'
**
** Created: Sat 12. Dec 14:45:38 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/gui/DialogParametrosCubo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DialogParametrosCubo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_simulacao__gui__DialogParametrosCubo[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      38,   37,   37,   37, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_simulacao__gui__DialogParametrosCubo[] = {
    "simulacao::gui::DialogParametrosCubo\0"
    "\0salvarAresta()\0"
};

const QMetaObject simulacao::gui::DialogParametrosCubo::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_simulacao__gui__DialogParametrosCubo,
      qt_meta_data_simulacao__gui__DialogParametrosCubo, 0 }
};

const QMetaObject *simulacao::gui::DialogParametrosCubo::metaObject() const
{
    return &staticMetaObject;
}

void *simulacao::gui::DialogParametrosCubo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_simulacao__gui__DialogParametrosCubo))
        return static_cast<void*>(const_cast< DialogParametrosCubo*>(this));
    return QDialog::qt_metacast(_clname);
}

int simulacao::gui::DialogParametrosCubo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: salvarAresta(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
