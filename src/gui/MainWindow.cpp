#include <QMessageBox>
#include <QtDebug>
#include "MainWindow.h"

using namespace simulacao::gui;
using namespace simulacao::canvas;

MainWindow::MainWindow(){
	ui = new Ui_MainWindow();
	ui->setupUi(this);
	
	ui->textNumeroGraos->setValidator(new QIntValidator(this));

	statusTipoSimulacao = new QLabel(tr(""));
	statusTipoSimulacao->setAlignment(Qt::AlignHCenter);
	statusTipoSimulacao->setMinimumSize(statusTipoSimulacao->sizeHint());

	statusQtdeObjetos = new QLabel(tr("0 Objetos"));
	statusQtdeObjetos->setAlignment(Qt::AlignHCenter);
	statusQtdeObjetos->setMinimumSize(statusQtdeObjetos->sizeHint());

	statusBar()->addWidget(statusTipoSimulacao);
    statusBar()->addWidget(statusQtdeObjetos, 1);


	criarCanvas();

	connect(ui->actionSobre, SIGNAL(triggered()), qApp, SLOT(aboutQt()));


}

MainWindow::~MainWindow(){

}

void MainWindow::criarCanvas(){

	simulacao = new SimulacaoCaixa;

	view = new CaixaGraosGLWidget(this,simulacao);
	view->setFocusPolicy(Qt::StrongFocus);
	view->setObjectName(QString::fromUtf8("graphicsView"));

	QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy2.setHorizontalStretch(0);
	sizePolicy2.setVerticalStretch(0);
	sizePolicy2.setHeightForWidth(view->sizePolicy().hasHeightForWidth());
	view->setSizePolicy(sizePolicy2);
	view->setAcceptDrops(true);

	if (simulacao->isSimulacaoEmHardware())
		statusTipoSimulacao->setText(tr("Simulação em Hardware"));
	else
		statusTipoSimulacao->setText(tr("Simulação em Software"));

	ui->horizontalLayout_2->addWidget(view);
}

void MainWindow::configurarParametros(){

	switch(ui->comboBoxTipoGrao->currentIndex())
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		dialogParametrosCubo = new DialogParametrosCubo(this->simulacao);
		dialogParametrosCubo->setModal(true);
		dialogParametrosCubo->setVisible(true);
		break;
	}

}
void MainWindow::adicionarObjetos(){

	long qtde =  ui->textNumeroGraos->text().toLong();

	switch(ui->comboBoxTipoGrao->currentIndex())
	{
	case 0:
		simulacao->adicionarObjeto(ESFERA,qtde);
		break;
	case 1:
		simulacao->adicionarObjeto(PRISMA_TRIANGULAR,qtde);
		break;
	case 2:
		simulacao->adicionarObjeto(PRISMA_TRIANGULAR_TRUNCADO,qtde);
		break;
	case 3:
		simulacao->adicionarObjeto(CUBO,qtde);
		break;
	}

	

	atualizarQuantidadeDeGraosEmCena();
		

}

void MainWindow::pararSimulacao(){
	//QMessageBox::warning(this, tr("teste"),tr("pararSimulacao"));
	if (simulacao->getStatus() == PAUSADO){
		
		simulacao->setStatus(EXECUCAO);
		ui->btnParar->setText(tr("Pausar"));
	}
	else{
		simulacao->setStatus(PAUSADO);
		ui->btnParar->setText(tr("Continuar"));
	}

}

void MainWindow::limparSimulacao(){
	simulacao->removerGraos();
	atualizarQuantidadeDeGraosEmCena();

}

void MainWindow::novoPlanoDeCorte(){
	simulacao->novoPlanoDeCorte();

}

void MainWindow::exibirSobre(){
	QMessageBox::warning(this, tr("teste"),tr("exibirSobre"));
}

void MainWindow::exibirPlanoDeCorte(bool b){
	if(b)
		simulacao->exibirPlanoDeCorte();
	else
		simulacao->esconderPlanoDeCorte();
}
void MainWindow::usarGravidade(bool b){
	if (b)
		simulacao->habilitarGravidade();
	else
		simulacao->desabilitarGravidade();
}

void MainWindow::mostrarCaixa(bool b){
}

void MainWindow::usarTamanhoDeGraoAleatorio(bool b){
}

/** Exibe os objetos que estão sendo interceptados pelo plano */
void MainWindow::exibirGraosInterceptados(){
	simulacao->selecionarGraosInterceptados();
	atualizarQuantidadeDeGraosEmCena();
}

/** Exibe as regiões no plano interceptadas pelos objetos cortados pelo plano */
void MainWindow::exibirInterceptos(){
	simulacao->selecionarInterceptacoes();
	
}

void MainWindow::atualizarQuantidadeDeGraosEmCena(){
	long qtde_total_objetos = simulacao->getQtdeObjetos() - 2; // -2 remove a caixa e o plano 
	statusQtdeObjetos->setText(tr("%1 Objetos").arg(qtde_total_objetos));
}