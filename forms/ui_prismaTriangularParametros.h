/********************************************************************************
** Form generated from reading ui file 'prismaTriangularParametros.ui'
**
** Created: Sat 12. Dec 21:09:11 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PRISMATRIANGULARPARAMETROS_H
#define UI_PRISMATRIANGULARPARAMETROS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_WinParametrosPrismaTriangular
{
public:
    QGroupBox *groupBox;
    QLineEdit *textRaio;
    QLabel *label;
    QLineEdit *textRaio_2;
    QLabel *label_2;
    QPushButton *btnSalvar;
    QPushButton *btnCancelar;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(276, 141);
        groupBox = new QGroupBox(Dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 251, 91));
        textRaio = new QLineEdit(groupBox);
        textRaio->setObjectName(QString::fromUtf8("textRaio"));
        textRaio->setGeometry(QRect(60, 30, 131, 20));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 33, 46, 14));
        textRaio_2 = new QLineEdit(groupBox);
        textRaio_2->setObjectName(QString::fromUtf8("textRaio_2"));
        textRaio_2->setGeometry(QRect(60, 57, 131, 20));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 60, 46, 14));
        btnSalvar = new QPushButton(Dialog);
        btnSalvar->setObjectName(QString::fromUtf8("btnSalvar"));
        btnSalvar->setGeometry(QRect(190, 110, 75, 23));
        btnCancelar = new QPushButton(Dialog);
        btnCancelar->setObjectName(QString::fromUtf8("btnCancelar"));
        btnCancelar->setGeometry(QRect(110, 110, 75, 23));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("WinParametrosPrismaTriangular", "Par\303\242metros - Prisma Triangular", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("WinParametrosPrismaTriangular", "Dados do Prisma Tringular", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("WinParametrosPrismaTriangular", "Altura:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("WinParametrosPrismaTriangular", "Base:", 0, QApplication::UnicodeUTF8));
        btnSalvar->setText(QApplication::translate("WinParametrosPrismaTriangular", "Salvar", 0, QApplication::UnicodeUTF8));
        btnCancelar->setText(QApplication::translate("WinParametrosPrismaTriangular", "Cancelar", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Dialog);
    } // retranslateUi

};

QT_END_NAMESPACE

#endif // UI_PRISMATRIANGULARPARAMETROS_H
