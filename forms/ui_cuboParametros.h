/********************************************************************************
** Form generated from reading ui file 'cuboParametros.ui'
**
** Created: Fri 18. Dec 13:38:21 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CUBOPARAMETROS_H
#define UI_CUBOPARAMETROS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_WinParametrosCubo
{
public:
    QGroupBox *groupBox;
    QLineEdit *textAresta;
    QPushButton *btnSalvar;
    QPushButton *btnCancelar;

    void setupUi(QDialog *WinParametrosCubo)
    {
        if (WinParametrosCubo->objectName().isEmpty())
            WinParametrosCubo->setObjectName(QString::fromUtf8("WinParametrosCubo"));
        WinParametrosCubo->resize(242, 100);
        groupBox = new QGroupBox(WinParametrosCubo);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 221, 51));
        textAresta = new QLineEdit(groupBox);
        textAresta->setObjectName(QString::fromUtf8("textAresta"));
        textAresta->setGeometry(QRect(10, 20, 131, 20));
        btnSalvar = new QPushButton(WinParametrosCubo);
        btnSalvar->setObjectName(QString::fromUtf8("btnSalvar"));
        btnSalvar->setGeometry(QRect(160, 70, 75, 23));
        btnCancelar = new QPushButton(WinParametrosCubo);
        btnCancelar->setObjectName(QString::fromUtf8("btnCancelar"));
        btnCancelar->setGeometry(QRect(80, 70, 75, 23));

        retranslateUi(WinParametrosCubo);
        QObject::connect(btnSalvar, SIGNAL(clicked()), WinParametrosCubo, SLOT(salvarAresta()));
        QObject::connect(btnCancelar, SIGNAL(clicked()), WinParametrosCubo, SLOT(close()));

        QMetaObject::connectSlotsByName(WinParametrosCubo);
    } // setupUi

    void retranslateUi(QDialog *WinParametrosCubo)
    {
        WinParametrosCubo->setWindowTitle(QApplication::translate("WinParametrosCubo", "Par\303\242metros - Cubo", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("WinParametrosCubo", "Aresta do Cubo", 0, QApplication::UnicodeUTF8));
        btnSalvar->setText(QApplication::translate("WinParametrosCubo", "Salvar", 0, QApplication::UnicodeUTF8));
        btnCancelar->setText(QApplication::translate("WinParametrosCubo", "Cancelar", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(WinParametrosCubo);
    } // retranslateUi

};

QT_END_NAMESPACE

#endif // UI_CUBOPARAMETROS_H
