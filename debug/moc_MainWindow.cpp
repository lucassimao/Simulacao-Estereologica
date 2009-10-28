/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created: Thu 8. Oct 21:45:35 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/gui/MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_simulacao__gui__MainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      28,   27,   27,   27, 0x08,
      47,   27,   27,   27, 0x08,
      64,   27,   27,   27, 0x08,
      82,   27,   27,   27, 0x08,
     101,   27,   27,   27, 0x08,
     115,   27,   27,   27, 0x08,
     142,   27,   27,   27, 0x08,
     167,   27,   27,   27, 0x08,
     187,   27,   27,   27, 0x08,
     206,   27,   27,   27, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_simulacao__gui__MainWindow[] = {
    "simulacao::gui::MainWindow\0\0"
    "adicionarObjetos()\0pararSimulacao()\0"
    "limparSimulacao()\0novoPlanoDeCorte()\0"
    "exibirSobre()\0exibirGraosInterceptados()\0"
    "exibirPlanoDeCorte(bool)\0usarGravidade(bool)\0"
    "mostrarCaixa(bool)\0usarTamanhoDeGraoAleatorio(bool)\0"
};

const QMetaObject simulacao::gui::MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_simulacao__gui__MainWindow,
      qt_meta_data_simulacao__gui__MainWindow, 0 }
};

const QMetaObject *simulacao::gui::MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *simulacao::gui::MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_simulacao__gui__MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int simulacao::gui::MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: adicionarObjetos(); break;
        case 1: pararSimulacao(); break;
        case 2: limparSimulacao(); break;
        case 3: novoPlanoDeCorte(); break;
        case 4: exibirSobre(); break;
        case 5: exibirGraosInterceptados(); break;
        case 6: exibirPlanoDeCorte((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: usarGravidade((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: mostrarCaixa((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: usarTamanhoDeGraoAleatorio((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
