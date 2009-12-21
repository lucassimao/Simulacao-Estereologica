/********************************************************************************
** Form generated from reading ui file 'esferaIdenticaParametros.ui'
**
** Created: Mon 21. Dec 13:32:36 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ESFERAIDENTICAPARAMETROS_H
#define UI_ESFERAIDENTICAPARAMETROS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DlgParametrosEsferasIdenticas
{
public:
    QPushButton *btnSalvar;
    QPushButton *btnCancelar;
    QLineEdit *textRaioDaEsfera;
    QLabel *label;
    QLineEdit *textPorcentagemDeVazio;
    QLabel *label_3;
    QLineEdit *textNumeroDeGraos;
    QLabel *label_4;
    QLabel *label_5;
    QWidget *widgetCor;

    void setupUi(QDialog *DlgParametrosEsferasIdenticas)
    {
        if (DlgParametrosEsferasIdenticas->objectName().isEmpty())
            DlgParametrosEsferasIdenticas->setObjectName(QString::fromUtf8("DlgParametrosEsferasIdenticas"));
        DlgParametrosEsferasIdenticas->resize(280, 172);
        DlgParametrosEsferasIdenticas->setModal(true);
        btnSalvar = new QPushButton(DlgParametrosEsferasIdenticas);
        btnSalvar->setObjectName(QString::fromUtf8("btnSalvar"));
        btnSalvar->setGeometry(QRect(180, 140, 75, 23));
        btnSalvar->setCursor(QCursor(Qt::PointingHandCursor));
        btnCancelar = new QPushButton(DlgParametrosEsferasIdenticas);
        btnCancelar->setObjectName(QString::fromUtf8("btnCancelar"));
        btnCancelar->setGeometry(QRect(100, 140, 75, 23));
        btnCancelar->setCursor(QCursor(Qt::PointingHandCursor));
        textRaioDaEsfera = new QLineEdit(DlgParametrosEsferasIdenticas);
        textRaioDaEsfera->setObjectName(QString::fromUtf8("textRaioDaEsfera"));
        textRaioDaEsfera->setGeometry(QRect(140, 20, 113, 20));
        label = new QLabel(DlgParametrosEsferasIdenticas);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 111, 16));
        textPorcentagemDeVazio = new QLineEdit(DlgParametrosEsferasIdenticas);
        textPorcentagemDeVazio->setObjectName(QString::fromUtf8("textPorcentagemDeVazio"));
        textPorcentagemDeVazio->setGeometry(QRect(140, 50, 113, 20));
        label_3 = new QLabel(DlgParametrosEsferasIdenticas);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 50, 111, 16));
        textNumeroDeGraos = new QLineEdit(DlgParametrosEsferasIdenticas);
        textNumeroDeGraos->setObjectName(QString::fromUtf8("textNumeroDeGraos"));
        textNumeroDeGraos->setGeometry(QRect(140, 80, 113, 20));
        label_4 = new QLabel(DlgParametrosEsferasIdenticas);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 80, 111, 16));
        label_5 = new QLabel(DlgParametrosEsferasIdenticas);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 110, 46, 14));
        widgetCor = new QWidget(DlgParametrosEsferasIdenticas);
        widgetCor->setObjectName(QString::fromUtf8("widgetCor"));
        widgetCor->setGeometry(QRect(140, 110, 111, 21));
        widgetCor->setCursor(QCursor(Qt::PointingHandCursor));

        retranslateUi(DlgParametrosEsferasIdenticas);
        QObject::connect(btnCancelar, SIGNAL(clicked()), DlgParametrosEsferasIdenticas, SLOT(cancelar()));
        QObject::connect(btnSalvar, SIGNAL(clicked()), DlgParametrosEsferasIdenticas, SLOT(salvar()));

        QMetaObject::connectSlotsByName(DlgParametrosEsferasIdenticas);
    } // setupUi

    void retranslateUi(QDialog *DlgParametrosEsferasIdenticas)
    {
        DlgParametrosEsferasIdenticas->setWindowTitle(QApplication::translate("DlgParametrosEsferasIdenticas", "Par\303\242metros - Esfera", 0, QApplication::UnicodeUTF8));
        btnSalvar->setText(QApplication::translate("DlgParametrosEsferasIdenticas", "Salvar", 0, QApplication::UnicodeUTF8));
        btnCancelar->setText(QApplication::translate("DlgParametrosEsferasIdenticas", "Cancelar", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DlgParametrosEsferasIdenticas", "Raio da esfera:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DlgParametrosEsferasIdenticas", "Porcentagem de vazio:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DlgParametrosEsferasIdenticas", "N\303\272mero de gr\303\243os:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DlgParametrosEsferasIdenticas", "Cor:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DlgParametrosEsferasIdenticas);
    } // retranslateUi

};

QT_END_NAMESPACE

#endif // UI_ESFERAIDENTICAPARAMETROS_H
