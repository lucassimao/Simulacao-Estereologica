/********************************************************************************
** Form generated from reading ui file 'esferaParametros.ui'
**
** Created: Sun 13. Dec 12:58:14 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ESFERAPARAMETROS_H
#define UI_ESFERAPARAMETROS_H

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

class Ui_WinParametrosEsfera
{
public:
    QGroupBox *groupBox;
    QLineEdit *textRaio;
    QPushButton *btnSalvar;
    QPushButton *btnCancelar;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(242, 100);
        groupBox = new QGroupBox(Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 221, 51));
        textRaio = new QLineEdit(groupBox);
        textRaio->setObjectName(QString::fromUtf8("textRaio"));
        textRaio->setGeometry(QRect(10, 20, 131, 20));
        btnSalvar = new QPushButton(Dialog);
        btnSalvar->setObjectName(QString::fromUtf8("btnSalvar"));
        btnSalvar->setGeometry(QRect(160, 70, 75, 23));
        btnCancelar = new QPushButton(Dialog);
        btnCancelar->setObjectName(QString::fromUtf8("btnCancelar"));
        btnCancelar->setGeometry(QRect(80, 70, 75, 23));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("WinParametrosEsfera", "Par\303\242metros - Esfera", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("WinParametrosEsfera", "Raio da Esfera ", 0, QApplication::UnicodeUTF8));
        btnSalvar->setText(QApplication::translate("WinParametrosEsfera", "Salvar", 0, QApplication::UnicodeUTF8));
        btnCancelar->setText(QApplication::translate("WinParametrosEsfera", "Cancelar", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Dialog);
    } // retranslateUi

};

QT_END_NAMESPACE

#endif // UI_ESFERAPARAMETROS_H
