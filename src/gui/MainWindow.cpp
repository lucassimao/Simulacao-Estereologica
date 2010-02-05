#include <QMessageBox>
#include <QtDebug>
#include <QInputDialog>
#include "MainWindow.h"

using namespace simulacao::model;
using namespace simulacao::gui;
using namespace simulacao::canvas;

#include "DialogParametrosCubo.h"
#include "DialogParametrosGrade.h"
#include "..\model\Parametros.h"
#include "..\canvas\glWidget\RenderizacaoStrategy.h"
#include "..\canvas\glWidget\RenderizarAtoresStrategy.h"
#include "..\canvas\glWidget\RenderizarInterceptosStrategy.h"
using namespace simulacao::canvas::glWidget;

#include "..\math\ColetorDeAreasVisitor.h"
#include "..\math\ColetorDeInterceptosLinearesVisitor.h"

using namespace simulacao::math::mathVisitor;

#include <fstream>
using std::ofstream;

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

void MainWindow::configurarGrade(){
		DialogParametrosGrade *dialog = new DialogParametrosGrade(this,this->simulacao);
		dialog->setModal(true);
		dialog->setVisible(true);

}

inline void MainWindow::criarCanvas(){

	simulacao = new SimulacaoCaixa;
	RenderizacaoStrategy *strategy = new RenderizarAtoresStrategy();
	
	view = new CaixaGraosGLWidget(this,simulacao);
	view->setStrategy(strategy);
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

	int res;
	QInputDialog *dlg = new QInputDialog(this);
	dlg->setDoubleMinimum(0.1);
	dlg->setInputMode(QInputDialog::InputMode::DoubleInput);

	switch(ui->comboBoxTipoGrao->currentIndex())
	{
	case 0:
		
		dlg->setLabelText(tr("Raio da esfera:"));
		dlg->setDoubleValue(Parametros::getInstance()->getRaioEsfera());
		res = dlg->exec();

		if (res == QInputDialog::DialogCode::Accepted){
			Parametros::getInstance()->setRaioEsfera(dlg->doubleValue());
		}
		break;
	case 1:
		dlg->setLabelText(tr("Altura do Prisma:"));
		dlg->setDoubleValue(Parametros::getInstance()->getAlturaPrisma());
		res = dlg->exec();

		if (res == QInputDialog::DialogCode::Accepted){
			Parametros::getInstance()->setAlturaPrisma(dlg->doubleValue());

			dlg->setLabelText(tr("Base do Prisma:"));
			dlg->setDoubleValue(Parametros::getInstance()->getBasePrisma());
			res = dlg->exec();

			if (res == QInputDialog::DialogCode::Accepted){
				Parametros::getInstance()->setBasePrisma(dlg->doubleValue());
			}
		}

		break;
	case 2:
		break;
	case 3:
		dlg->setLabelText(tr("Aresta do cubo:"));
		dlg->setDoubleValue(Parametros::getInstance()->getArestaCubo());
		res = dlg->exec();

		if (res == QInputDialog::DialogCode::Accepted){
			Parametros::getInstance()->setArestaCubo(dlg->doubleValue());
		}
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

void MainWindow::novaSimulacao(){
	this->ui->horizontalLayout_2->removeWidget(view);
	delete view;
	this->simulacao->pararSimulacao();
	this->simulacao = new SimulacaoCaixa;
	this->simulacao->novoPlanoDeCorte();

	ui->btnParar->setText("Pausar");
	ui->btnNovoPlanodeCorte->setEnabled(true);
	ui->btnPlanovsGraos->setEnabled(true);
	ui->btnExibirInterceptos->setEnabled(false);

	ui->checkBoxExibirPlanoDeCorte->setChecked(true);
	ui->checkBoxExibirPontosTeste->setChecked(true);
	ui->checkBoxExibirRetasTeste->setChecked(true);
	
	criarCanvas();
	atualizarQuantidadeDeGraosEmCena();
	
}

void MainWindow::usarGraosAleatorios(bool b){
	Parametros::setUsarGraosAleatorios(b);
}

/** Exibe os objetos que estão sendo interceptados pelo plano */
void MainWindow::exibirGraosInterceptados(){
	
	simulacao->selecionarGraosInterceptados();
	atualizarQuantidadeDeGraosEmCena();

	ui->btnNovoPlanodeCorte->setEnabled(false);
	ui->btnExibirInterceptos->setEnabled(true);
	ui->btnPlanovsGraos->setEnabled(false);
}

/** Exibe as regiões no plano interceptadas pelos objetos cortados pelo plano */
void MainWindow::exibirInterceptos(){
	Grade *g = simulacao->getGrade();

	RenderizarInterceptosStrategy *strategy = new RenderizarInterceptosStrategy(g);
	view->setStrategy(strategy);
	ui->btnExibirInterceptos->setEnabled(false);

}

void MainWindow::exibirRetasTeste(bool b){
	this->simulacao->setExibirRetasTeste(b);
}
void MainWindow::exibirPontosTeste(bool b){
	this->simulacao->setExibirPontosTeste(b);
}

inline void MainWindow::atualizarQuantidadeDeGraosEmCena(){
	long qtde_total_objetos = simulacao->getQtdeObjetos() - 2; // -2 remove a caixa e o plano 
	statusQtdeObjetos->setText(tr("%1 Objetos").arg(qtde_total_objetos));
}