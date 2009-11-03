/********************************************************************************
** Form generated from reading ui file 'gui.ui'
**
** Created: Tue 3. Nov 12:55:42 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GUI_H
#define UI_GUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSalvar_Figura;
    QAction *actionSalvar_resultados;
    QAction *actionSalvar_cena;
    QAction *actionAbrir_cena;
    QAction *actionSair;
    QAction *actionEsferas_id_nticas;
    QAction *actionEsferas_aleat_rias;
    QAction *actionPercola_o;
    QAction *actionCores_dos_gr_os;
    QAction *actionPosi_o;
    QAction *actionGrade_de_pontos_e_linha_teste;
    QAction *actionPlano_de_corte;
    QAction *actionExibir_resultados;
    QAction *actionDistribui_o_de_gr_os;
    QAction *actionAglomerados;
    QAction *actionPlano_de_fundo;
    QAction *actionTetaedro;
    QAction *actionEctaedro;
    QAction *actionCubo_simples;
    QAction *actionC_bico_de_corpo_centrado;
    QAction *actionCores;
    QAction *actionAjuda;
    QAction *actionSobre;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QComboBox *comboBoxTipoGrao;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *textNumeroGraos;
    QPushButton *btnAdicionar;
    QSpacerItem *horizontalSpacer_3;
    QCheckBox *checkBoxTamanhoGraoAleatorio;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *textRaioEsfera;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *btnParar;
    QPushButton *btnLimpar;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *checkBoxVistaSuperior;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_4;
    QSpinBox *spinBoxZoom;
    QSpacerItem *horizontalSpacer_4;
    QCheckBox *checkBoxMostrarCaixa;
    QCheckBox *checkBoxCaixaSemTampa;
    QCheckBox *checkBoxGravidade;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *btnNovoPlanodeCorte;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *btnPlanovsGraos;
    QPushButton *btnAmostra;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *btnGraosInterceptos;
    QPushButton *btnExibirInterceptos;
    QCheckBox *checkBoxExibirPlanoDeCorte;
    QCheckBox *checkBoxExibirRetasTeste;
    QCheckBox *checkBoxExibirPontosTeste;
    QSpacerItem *verticalSpacer;
    QMenuBar *menubar;
    QMenu *menuArquivo;
    QMenu *menuOp_es;
    QMenu *menuPoros_CellUnit;
    QMenu *menuAjuda;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(852, 609);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(800, 600));
        QIcon icon;
        icon.addPixmap(QPixmap(QString::fromUtf8("../../../../Qt/2009.02/share/pixmaps/qtcreator_logo_24.png")), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setDocumentMode(false);
        actionSalvar_Figura = new QAction(MainWindow);
        actionSalvar_Figura->setObjectName(QString::fromUtf8("actionSalvar_Figura"));
        actionSalvar_resultados = new QAction(MainWindow);
        actionSalvar_resultados->setObjectName(QString::fromUtf8("actionSalvar_resultados"));
        actionSalvar_cena = new QAction(MainWindow);
        actionSalvar_cena->setObjectName(QString::fromUtf8("actionSalvar_cena"));
        actionAbrir_cena = new QAction(MainWindow);
        actionAbrir_cena->setObjectName(QString::fromUtf8("actionAbrir_cena"));
        actionSair = new QAction(MainWindow);
        actionSair->setObjectName(QString::fromUtf8("actionSair"));
        actionEsferas_id_nticas = new QAction(MainWindow);
        actionEsferas_id_nticas->setObjectName(QString::fromUtf8("actionEsferas_id_nticas"));
        actionEsferas_aleat_rias = new QAction(MainWindow);
        actionEsferas_aleat_rias->setObjectName(QString::fromUtf8("actionEsferas_aleat_rias"));
        actionPercola_o = new QAction(MainWindow);
        actionPercola_o->setObjectName(QString::fromUtf8("actionPercola_o"));
        actionCores_dos_gr_os = new QAction(MainWindow);
        actionCores_dos_gr_os->setObjectName(QString::fromUtf8("actionCores_dos_gr_os"));
        actionPosi_o = new QAction(MainWindow);
        actionPosi_o->setObjectName(QString::fromUtf8("actionPosi_o"));
        actionGrade_de_pontos_e_linha_teste = new QAction(MainWindow);
        actionGrade_de_pontos_e_linha_teste->setObjectName(QString::fromUtf8("actionGrade_de_pontos_e_linha_teste"));
        actionPlano_de_corte = new QAction(MainWindow);
        actionPlano_de_corte->setObjectName(QString::fromUtf8("actionPlano_de_corte"));
        actionExibir_resultados = new QAction(MainWindow);
        actionExibir_resultados->setObjectName(QString::fromUtf8("actionExibir_resultados"));
        actionDistribui_o_de_gr_os = new QAction(MainWindow);
        actionDistribui_o_de_gr_os->setObjectName(QString::fromUtf8("actionDistribui_o_de_gr_os"));
        actionAglomerados = new QAction(MainWindow);
        actionAglomerados->setObjectName(QString::fromUtf8("actionAglomerados"));
        actionPlano_de_fundo = new QAction(MainWindow);
        actionPlano_de_fundo->setObjectName(QString::fromUtf8("actionPlano_de_fundo"));
        actionTetaedro = new QAction(MainWindow);
        actionTetaedro->setObjectName(QString::fromUtf8("actionTetaedro"));
        actionEctaedro = new QAction(MainWindow);
        actionEctaedro->setObjectName(QString::fromUtf8("actionEctaedro"));
        actionCubo_simples = new QAction(MainWindow);
        actionCubo_simples->setObjectName(QString::fromUtf8("actionCubo_simples"));
        actionC_bico_de_corpo_centrado = new QAction(MainWindow);
        actionC_bico_de_corpo_centrado->setObjectName(QString::fromUtf8("actionC_bico_de_corpo_centrado"));
        actionCores = new QAction(MainWindow);
        actionCores->setObjectName(QString::fromUtf8("actionCores"));
        actionAjuda = new QAction(MainWindow);
        actionAjuda->setObjectName(QString::fromUtf8("actionAjuda"));
        actionSobre = new QAction(MainWindow);
        actionSobre->setObjectName(QString::fromUtf8("actionSobre"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setMaximumSize(QSize(16777215, 16777215));
        centralwidget->setSizeIncrement(QSize(0, 0));
        centralwidget->setAcceptDrops(false);
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        groupBox->setMinimumSize(QSize(245, 0));
        groupBox->setMaximumSize(QSize(16777215, 16777215));
        groupBox->setBaseSize(QSize(0, 0));
        groupBox->setLayoutDirection(Qt::LeftToRight);
        groupBox->setAutoFillBackground(false);
        groupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        verticalLayoutWidget = new QWidget(groupBox);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 20, 221, 501));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        comboBoxTipoGrao = new QComboBox(verticalLayoutWidget);
        comboBoxTipoGrao->setObjectName(QString::fromUtf8("comboBoxTipoGrao"));
        comboBoxTipoGrao->setCursor(QCursor(Qt::PointingHandCursor));

        verticalLayout->addWidget(comboBoxTipoGrao);

        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        textNumeroGraos = new QLineEdit(verticalLayoutWidget);
        textNumeroGraos->setObjectName(QString::fromUtf8("textNumeroGraos"));

        horizontalLayout_3->addWidget(textNumeroGraos);

        btnAdicionar = new QPushButton(verticalLayoutWidget);
        btnAdicionar->setObjectName(QString::fromUtf8("btnAdicionar"));
        btnAdicionar->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_3->addWidget(btnAdicionar);

        horizontalSpacer_3 = new QSpacerItem(120, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_3);

        checkBoxTamanhoGraoAleatorio = new QCheckBox(verticalLayoutWidget);
        checkBoxTamanhoGraoAleatorio->setObjectName(QString::fromUtf8("checkBoxTamanhoGraoAleatorio"));
        checkBoxTamanhoGraoAleatorio->setCursor(QCursor(Qt::PointingHandCursor));
        checkBoxTamanhoGraoAleatorio->setTristate(false);

        verticalLayout->addWidget(checkBoxTamanhoGraoAleatorio);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        textRaioEsfera = new QLineEdit(verticalLayoutWidget);
        textRaioEsfera->setObjectName(QString::fromUtf8("textRaioEsfera"));

        horizontalLayout_4->addWidget(textRaioEsfera);

        horizontalSpacer = new QSpacerItem(108, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        btnParar = new QPushButton(verticalLayoutWidget);
        btnParar->setObjectName(QString::fromUtf8("btnParar"));
        btnParar->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_5->addWidget(btnParar);

        btnLimpar = new QPushButton(verticalLayoutWidget);
        btnLimpar->setObjectName(QString::fromUtf8("btnLimpar"));
        btnLimpar->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_5->addWidget(btnLimpar);

        horizontalSpacer_2 = new QSpacerItem(94, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_5);

        checkBoxVistaSuperior = new QCheckBox(verticalLayoutWidget);
        checkBoxVistaSuperior->setObjectName(QString::fromUtf8("checkBoxVistaSuperior"));
        checkBoxVistaSuperior->setCursor(QCursor(Qt::PointingHandCursor));

        verticalLayout->addWidget(checkBoxVistaSuperior);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_4 = new QLabel(verticalLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_6->addWidget(label_4);

        spinBoxZoom = new QSpinBox(verticalLayoutWidget);
        spinBoxZoom->setObjectName(QString::fromUtf8("spinBoxZoom"));
        spinBoxZoom->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_6->addWidget(spinBoxZoom);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_6);

        checkBoxMostrarCaixa = new QCheckBox(verticalLayoutWidget);
        checkBoxMostrarCaixa->setObjectName(QString::fromUtf8("checkBoxMostrarCaixa"));
        checkBoxMostrarCaixa->setCursor(QCursor(Qt::PointingHandCursor));
        checkBoxMostrarCaixa->setChecked(true);

        verticalLayout->addWidget(checkBoxMostrarCaixa);

        checkBoxCaixaSemTampa = new QCheckBox(verticalLayoutWidget);
        checkBoxCaixaSemTampa->setObjectName(QString::fromUtf8("checkBoxCaixaSemTampa"));
        checkBoxCaixaSemTampa->setCursor(QCursor(Qt::PointingHandCursor));

        verticalLayout->addWidget(checkBoxCaixaSemTampa);

        checkBoxGravidade = new QCheckBox(verticalLayoutWidget);
        checkBoxGravidade->setObjectName(QString::fromUtf8("checkBoxGravidade"));
        checkBoxGravidade->setCursor(QCursor(Qt::PointingHandCursor));
        checkBoxGravidade->setChecked(false);

        verticalLayout->addWidget(checkBoxGravidade);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        btnNovoPlanodeCorte = new QPushButton(verticalLayoutWidget);
        btnNovoPlanodeCorte->setObjectName(QString::fromUtf8("btnNovoPlanodeCorte"));
        btnNovoPlanodeCorte->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_9->addWidget(btnNovoPlanodeCorte);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        btnPlanovsGraos = new QPushButton(verticalLayoutWidget);
        btnPlanovsGraos->setObjectName(QString::fromUtf8("btnPlanovsGraos"));
        btnPlanovsGraos->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_7->addWidget(btnPlanovsGraos);

        btnAmostra = new QPushButton(verticalLayoutWidget);
        btnAmostra->setObjectName(QString::fromUtf8("btnAmostra"));
        btnAmostra->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_7->addWidget(btnAmostra);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        btnGraosInterceptos = new QPushButton(verticalLayoutWidget);
        btnGraosInterceptos->setObjectName(QString::fromUtf8("btnGraosInterceptos"));
        btnGraosInterceptos->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_8->addWidget(btnGraosInterceptos);

        btnExibirInterceptos = new QPushButton(verticalLayoutWidget);
        btnExibirInterceptos->setObjectName(QString::fromUtf8("btnExibirInterceptos"));
        btnExibirInterceptos->setCursor(QCursor(Qt::PointingHandCursor));

        horizontalLayout_8->addWidget(btnExibirInterceptos);


        verticalLayout->addLayout(horizontalLayout_8);

        checkBoxExibirPlanoDeCorte = new QCheckBox(verticalLayoutWidget);
        checkBoxExibirPlanoDeCorte->setObjectName(QString::fromUtf8("checkBoxExibirPlanoDeCorte"));
        checkBoxExibirPlanoDeCorte->setCursor(QCursor(Qt::PointingHandCursor));
        checkBoxExibirPlanoDeCorte->setChecked(true);

        verticalLayout->addWidget(checkBoxExibirPlanoDeCorte);

        checkBoxExibirRetasTeste = new QCheckBox(verticalLayoutWidget);
        checkBoxExibirRetasTeste->setObjectName(QString::fromUtf8("checkBoxExibirRetasTeste"));
        checkBoxExibirRetasTeste->setCursor(QCursor(Qt::PointingHandCursor));

        verticalLayout->addWidget(checkBoxExibirRetasTeste);

        checkBoxExibirPontosTeste = new QCheckBox(verticalLayoutWidget);
        checkBoxExibirPontosTeste->setObjectName(QString::fromUtf8("checkBoxExibirPontosTeste"));
        checkBoxExibirPontosTeste->setCursor(QCursor(Qt::PointingHandCursor));

        verticalLayout->addWidget(checkBoxExibirPontosTeste);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addWidget(groupBox);


        horizontalLayout->addLayout(horizontalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 852, 22));
        menuArquivo = new QMenu(menubar);
        menuArquivo->setObjectName(QString::fromUtf8("menuArquivo"));
        menuOp_es = new QMenu(menubar);
        menuOp_es->setObjectName(QString::fromUtf8("menuOp_es"));
        menuPoros_CellUnit = new QMenu(menubar);
        menuPoros_CellUnit->setObjectName(QString::fromUtf8("menuPoros_CellUnit"));
        menuAjuda = new QMenu(menubar);
        menuAjuda->setObjectName(QString::fromUtf8("menuAjuda"));
        MainWindow->setMenuBar(menubar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setSizeGripEnabled(true);
        MainWindow->setStatusBar(statusBar);
#ifndef QT_NO_SHORTCUT
        label->setBuddy(comboBoxTipoGrao);
        label_2->setBuddy(textNumeroGraos);
        label_3->setBuddy(textRaioEsfera);
#endif // QT_NO_SHORTCUT

        menubar->addAction(menuArquivo->menuAction());
        menubar->addAction(menuOp_es->menuAction());
        menubar->addAction(menuPoros_CellUnit->menuAction());
        menubar->addAction(menuAjuda->menuAction());
        menuArquivo->addAction(actionSalvar_Figura);
        menuArquivo->addAction(actionSalvar_resultados);
        menuArquivo->addAction(actionSalvar_cena);
        menuArquivo->addAction(actionAbrir_cena);
        menuArquivo->addSeparator();
        menuArquivo->addAction(actionSair);
        menuOp_es->addAction(actionEsferas_id_nticas);
        menuOp_es->addAction(actionEsferas_aleat_rias);
        menuOp_es->addAction(actionPercola_o);
        menuOp_es->addAction(actionCores_dos_gr_os);
        menuOp_es->addAction(actionPosi_o);
        menuOp_es->addAction(actionGrade_de_pontos_e_linha_teste);
        menuOp_es->addAction(actionPlano_de_corte);
        menuOp_es->addAction(actionExibir_resultados);
        menuOp_es->addAction(actionDistribui_o_de_gr_os);
        menuOp_es->addAction(actionAglomerados);
        menuOp_es->addAction(actionPlano_de_fundo);
        menuPoros_CellUnit->addAction(actionTetaedro);
        menuPoros_CellUnit->addAction(actionEctaedro);
        menuPoros_CellUnit->addAction(actionCubo_simples);
        menuPoros_CellUnit->addAction(actionC_bico_de_corpo_centrado);
        menuPoros_CellUnit->addSeparator();
        menuPoros_CellUnit->addAction(actionCores);
        menuAjuda->addAction(actionAjuda);
        menuAjuda->addAction(actionSobre);

        retranslateUi(MainWindow);
        QObject::connect(btnAdicionar, SIGNAL(clicked()), MainWindow, SLOT(adicionarObjetos()));
        QObject::connect(btnParar, SIGNAL(clicked()), MainWindow, SLOT(pararSimulacao()));
        QObject::connect(btnLimpar, SIGNAL(clicked()), MainWindow, SLOT(limparSimulacao()));
        QObject::connect(btnNovoPlanodeCorte, SIGNAL(clicked()), MainWindow, SLOT(novoPlanoDeCorte()));
        QObject::connect(actionSair, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(checkBoxTamanhoGraoAleatorio, SIGNAL(toggled(bool)), MainWindow, SLOT(usarTamanhoDeGraoAleatorio(bool)));
        QObject::connect(checkBoxMostrarCaixa, SIGNAL(toggled(bool)), MainWindow, SLOT(mostrarCaixa(bool)));
        QObject::connect(checkBoxGravidade, SIGNAL(toggled(bool)), MainWindow, SLOT(usarGravidade(bool)));
        QObject::connect(checkBoxExibirPlanoDeCorte, SIGNAL(toggled(bool)), MainWindow, SLOT(exibirPlanoDeCorte(bool)));
        QObject::connect(btnPlanovsGraos, SIGNAL(clicked()), MainWindow, SLOT(exibirGraosInterceptados()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Caixa de Gr\303\243os", 0, QApplication::UnicodeUTF8));
        actionSalvar_Figura->setText(QApplication::translate("MainWindow", "Salvar figura", 0, QApplication::UnicodeUTF8));
        actionSalvar_resultados->setText(QApplication::translate("MainWindow", "Salvar resultados", 0, QApplication::UnicodeUTF8));
        actionSalvar_cena->setText(QApplication::translate("MainWindow", "Salvar cena", 0, QApplication::UnicodeUTF8));
        actionAbrir_cena->setText(QApplication::translate("MainWindow", "Abrir cena", 0, QApplication::UnicodeUTF8));
        actionSair->setText(QApplication::translate("MainWindow", "Sair", 0, QApplication::UnicodeUTF8));
        actionEsferas_id_nticas->setText(QApplication::translate("MainWindow", "Esferas id\303\252nticas", 0, QApplication::UnicodeUTF8));
        actionEsferas_aleat_rias->setText(QApplication::translate("MainWindow", "Esferas aleat\303\263rias", 0, QApplication::UnicodeUTF8));
        actionPercola_o->setText(QApplication::translate("MainWindow", "Percola\303\247\303\243o", 0, QApplication::UnicodeUTF8));
        actionCores_dos_gr_os->setText(QApplication::translate("MainWindow", "Cores dos gr\303\243os", 0, QApplication::UnicodeUTF8));
        actionPosi_o->setText(QApplication::translate("MainWindow", "Posi\303\247\303\243o", 0, QApplication::UnicodeUTF8));
        actionGrade_de_pontos_e_linha_teste->setText(QApplication::translate("MainWindow", "Grade de pontos e linha teste", 0, QApplication::UnicodeUTF8));
        actionPlano_de_corte->setText(QApplication::translate("MainWindow", "Plano de corte", 0, QApplication::UnicodeUTF8));
        actionExibir_resultados->setText(QApplication::translate("MainWindow", "Exibir resultados", 0, QApplication::UnicodeUTF8));
        actionDistribui_o_de_gr_os->setText(QApplication::translate("MainWindow", "Distribui\303\247\303\243o de gr\303\243os", 0, QApplication::UnicodeUTF8));
        actionAglomerados->setText(QApplication::translate("MainWindow", "Aglomerados", 0, QApplication::UnicodeUTF8));
        actionPlano_de_fundo->setText(QApplication::translate("MainWindow", "Cor de fundo", 0, QApplication::UnicodeUTF8));
        actionTetaedro->setText(QApplication::translate("MainWindow", "Tetaedro", 0, QApplication::UnicodeUTF8));
        actionEctaedro->setText(QApplication::translate("MainWindow", "Octaedro", 0, QApplication::UnicodeUTF8));
        actionCubo_simples->setText(QApplication::translate("MainWindow", "C\303\272bico simples", 0, QApplication::UnicodeUTF8));
        actionC_bico_de_corpo_centrado->setText(QApplication::translate("MainWindow", "C\303\272bico de corpo centrado", 0, QApplication::UnicodeUTF8));
        actionCores->setText(QApplication::translate("MainWindow", "Cores", 0, QApplication::UnicodeUTF8));
        actionAjuda->setText(QApplication::translate("MainWindow", "Ajuda", 0, QApplication::UnicodeUTF8));
        actionSobre->setText(QApplication::translate("MainWindow", "Sobre ...", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Op\303\247\303\265es ", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Tipo de gr\303\243o", 0, QApplication::UnicodeUTF8));
        comboBoxTipoGrao->clear();
        comboBoxTipoGrao->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Esfera", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Paralelep\303\255pedo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Prisma Triangular", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Prisma Triangular (Mesh)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Prisma Hexagonal", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "C\303\241psula", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Cilindro", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Cuboctaedro", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Octaedro Truncado", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Cone", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Cubo", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("MainWindow", "N\303\272mero de Gr\303\243os", 0, QApplication::UnicodeUTF8));
        btnAdicionar->setText(QApplication::translate("MainWindow", "Adicionar", 0, QApplication::UnicodeUTF8));
        checkBoxTamanhoGraoAleatorio->setText(QApplication::translate("MainWindow", "Tamanho de Gr\303\243o Aleat\303\263rio", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Raio da Esfera", 0, QApplication::UnicodeUTF8));
        textRaioEsfera->setText(QApplication::translate("MainWindow", "0.9", 0, QApplication::UnicodeUTF8));
        btnParar->setText(QApplication::translate("MainWindow", "Pausar", 0, QApplication::UnicodeUTF8));
        btnLimpar->setText(QApplication::translate("MainWindow", "Limpar", 0, QApplication::UnicodeUTF8));
        checkBoxVistaSuperior->setText(QApplication::translate("MainWindow", "Vista Superior", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Zoom (%):", 0, QApplication::UnicodeUTF8));
        checkBoxMostrarCaixa->setText(QApplication::translate("MainWindow", "Mostrar Caixa", 0, QApplication::UnicodeUTF8));
        checkBoxCaixaSemTampa->setText(QApplication::translate("MainWindow", "Caixa Sem Tampa", 0, QApplication::UnicodeUTF8));
        checkBoxGravidade->setText(QApplication::translate("MainWindow", "Gravidade", 0, QApplication::UnicodeUTF8));
        btnNovoPlanodeCorte->setText(QApplication::translate("MainWindow", "Novo Plano de Corte", 0, QApplication::UnicodeUTF8));
        btnPlanovsGraos->setText(QApplication::translate("MainWindow", "Plano Vs Gr\303\243os", 0, QApplication::UnicodeUTF8));
        btnAmostra->setText(QApplication::translate("MainWindow", "Amostra", 0, QApplication::UnicodeUTF8));
        btnGraosInterceptos->setText(QApplication::translate("MainWindow", "Gr\303\243os Interceptos", 0, QApplication::UnicodeUTF8));
        btnExibirInterceptos->setText(QApplication::translate("MainWindow", "Exibir Interceptos", 0, QApplication::UnicodeUTF8));
        checkBoxExibirPlanoDeCorte->setText(QApplication::translate("MainWindow", "Exibir Plano de Corte", 0, QApplication::UnicodeUTF8));
        checkBoxExibirRetasTeste->setText(QApplication::translate("MainWindow", "Exibir Retas Teste", 0, QApplication::UnicodeUTF8));
        checkBoxExibirPontosTeste->setText(QApplication::translate("MainWindow", "Exibir Pontos Teste", 0, QApplication::UnicodeUTF8));
        menuArquivo->setTitle(QApplication::translate("MainWindow", "Arquivo", 0, QApplication::UnicodeUTF8));
        menuOp_es->setTitle(QApplication::translate("MainWindow", "Op\303\247\303\265es", 0, QApplication::UnicodeUTF8));
        menuPoros_CellUnit->setTitle(QApplication::translate("MainWindow", "Poros CellUnit", 0, QApplication::UnicodeUTF8));
        menuAjuda->setTitle(QApplication::translate("MainWindow", "Ajuda", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        statusBar->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
    } // retranslateUi

};

QT_END_NAMESPACE

#endif // UI_GUI_H
