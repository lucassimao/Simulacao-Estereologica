#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>
#include <QColorDialog>
#include <QColor>
#include <string>
#include "MainWindow.h"

using std::string;
using namespace simulacao::model;
using namespace simulacao::gui;
using namespace simulacao::canvas;

#include "DistribuicaoLogNormalDialog.h"
#include "AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog.h"
#include "AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog.h"
#include "AdicionarEsferasSistematicamenteDialog.h"
#include "AdicionarPrismasSistematicamenteDialog.h"
#include "DialogParametrosGrade.h"
#include "DistribuicaoDeInterceptosDialog.h"
#include "..\model\Parametros.h"
#include "..\canvas\glWidget\RenderizacaoStrategy.h"
#include "..\canvas\glWidget\RenderizarAtoresStrategy.h"
#include "..\canvas\glWidget\RenderizarInterceptosStrategy.h"
using namespace simulacao::canvas::glWidget;

#include "..\math\ColetorDeAreasVisitor.h"
#include "..\math\ColetorDeInterceptosLinearesVisitor.h"
#include "..\exportador\ExportadorParaArquivo.h"
#include "..\exportador\ExportadorParaImagem.h"
#include "..\exportador\ProcessadorDeClassesDeIntercepto.h"

using namespace simulacao::exportador;
using namespace simulacao::math;
using namespace simulacao::math::mathVisitor;


#include <fstream>
using std::ofstream;


#include "..\utils\GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h"
#include "..\utils\GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy.h"

using namespace simulacao::model;

#include "..\sqlite3\sqlite3.h"

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
	DialogParametrosGrade *dialog = new DialogParametrosGrade(this);
	dialog->setModal(true);
	dialog->setVisible(true);
}

void MainWindow::exibirDistribuicaoDeInterceptos(){
	int res;

	QInputDialog *dlg = new QInputDialog(this);

	dlg->setIntMinimum(1);
	dlg->setIntMaximum(1000000);
	dlg->setInputMode(QInputDialog::InputMode::IntInput);
	dlg->setLabelText(tr("Quantidade de planos de corte:"));
	dlg->setIntValue(10);
	res = dlg->exec();

	if (res == QInputDialog::DialogCode::Accepted){
		int qtdeDePlanosDeCorte = dlg->intValue();
		sqlite3* db = this->simulacao->executarCortesSistematicos(qtdeDePlanosDeCorte);		
		DistribuicaoDeInterceptosDialog *dlg = new DistribuicaoDeInterceptosDialog(this,db);
		dlg->exec();

		sqlite3_close(db);
	}

}

void MainWindow::distribuicaoLogNormal(){
	DistribuicaoLogNormalDialog *dlg = new DistribuicaoLogNormalDialog(this,this->simulacao);
	int result =  dlg->exec();

	if (result ==  QInputDialog::DialogCode::Accepted)
	{
		AdicionarObjetosCommand *command = dlg->getCommand();
		if (command){
			command->execute();
			atualizarQuantidadeDeGraosEmCena();
		}
	}
}

void MainWindow::adicionarEsferasSistematicamente(){
	AdicionarEsferasSistematicamenteDialog *dlg = new AdicionarEsferasSistematicamenteDialog(this,this->simulacao);
	int result =  dlg->exec();
	if (result ==  QInputDialog::DialogCode::Accepted)
	{
		AdicionarObjetosCommand *command = dlg->getCommand();
		if (command){
			command->execute();
			atualizarQuantidadeDeGraosEmCena();
		}
	}
}

void MainWindow::configurarCorDoPlanoDeCorte(){
	QColorDialog dialog;
	QColor cor = dialog.getColor(Qt::green,this,"Selecione a cor para o plano de corte");
	if (cor.isValid()){
		Cor c = {cor.red()/255.0f,cor.green()/255.0f,cor.blue()/255.0f};
		simulacao->getPlanoDeCorte()->cor = c;
	}
}

void MainWindow::actionExecutarCortesSistematicos(){

	QInputDialog *dlg = new QInputDialog(this);
	dlg->setIntMinimum(1);
	dlg->setIntMaximum(100000000);
	dlg->setInputMode(QInputDialog::InputMode::IntInput);
	dlg->setLabelText(tr("Quantidade de planos de corte:"));
	dlg->setIntValue(1);
	int res = dlg->exec();

	if (res == QInputDialog::DialogCode::Accepted){
		int qtdeDePlanos = dlg->intValue();

		dlg->setLabelText(tr("Quantidade de classes de intercepto:"));
		dlg->setCancelButtonText(tr("Não Gerar Distribuição de grãos"));

		res = dlg->exec();
		bool gerarDistribuicaoDeInterceptos = (res == QInputDialog::DialogCode::Accepted);
		int qtdeDeClassesDeIntercepto = dlg->intValue();

		QString dir = QFileDialog::getExistingDirectory(this,"Selecione o diretório onde deseja que as informações sejam salvas");

		if (dir.trimmed().size()>0){
			sqlite3 *db = this->simulacao->executarCortesSistematicos(qtdeDePlanos);

			ExportadorParaArquivo exportador1(dir.toStdString(),db);
			exportador1.exportarPlanosDeCorte();
			if (gerarDistribuicaoDeInterceptos){
				exportador1.exportarTabelasDeProbabilidade(qtdeDeClassesDeIntercepto);
			}

			res = QMessageBox::question(this, tr("Pergunta"),tr("Deseja exportar também as imagens de cada plano de corte?"),
				QMessageBox::StandardButton::Ok|QMessageBox::StandardButton::No);

			bool exportarImagens = (res == QMessageBox::StandardButton::Ok);
			if (exportarImagens){
				ExportadorParaImagem exportador2(dir.toStdString(),db);
				exportador2.exportar();
			}
			sqlite3_close(db);

			QMessageBox::information(this, tr("Exportação concluída"),tr("Os dados foram exportados com sucesso!"));
		}

	}

}

void MainWindow::adicionarEsferasSistematicamenteComVolumeDaCaixaAjustavel(){
	AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog *dlg = new AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog(this,this->simulacao);
	int result =  dlg->exec();
	if (result ==  QInputDialog::DialogCode::Accepted)
	{
		AdicionarObjetosCommand *command = dlg->getCommand();
		if (command){
			command->execute();
			atualizarQuantidadeDeGraosEmCena();
		}
	}
}

void MainWindow::adicionarPrismasSistematicamenteComVolumeDaCaixaAjustavel(){
	AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog *dlg = new AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog(this,this->simulacao);
	int result =  dlg->exec();
	if (result ==  QInputDialog::DialogCode::Accepted)
	{
		AdicionarObjetosCommand *command = dlg->getCommand();
		if (command){
			command->execute();
			atualizarQuantidadeDeGraosEmCena();
		}
	}
}

void MainWindow::adicionarPrismasSistematicamente(){
	AdicionarPrismasSistematicamenteDialog *dlg = new AdicionarPrismasSistematicamenteDialog(this,this->simulacao);
	int result =  dlg->exec();
	if (result ==  QInputDialog::DialogCode::Accepted)
	{
		AdicionarObjetosCommand *command = dlg->getCommand();
		if (command){
			command->execute();
			atualizarQuantidadeDeGraosEmCena();
		}
	}		
}


void MainWindow::exibirVisaoSuperior(bool b){
	if(b){
		view->posicionarCameraNoTopoDaCaixa();
	}
	else
	{
		view->posicionarCameraNoPontoInicial();
	}
}

inline void MainWindow::criarCanvas(){

	simulacao = new SimulacaoCaixa;
	simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy());


	view = new CaixaGraosGLWidget(this,simulacao);
	view->setStrategy(new RenderizarAtoresStrategy());
	view->setFocusPolicy(Qt::StrongFocus);
	view->setObjectName(QString::fromUtf8("graphicsView"));

	QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy2.setHorizontalStretch(0);
	sizePolicy2.setVerticalStretch(0);
	sizePolicy2.setHeightForWidth(view->sizePolicy().hasHeightForWidth());
	view->setSizePolicy(sizePolicy2);
	view->setAcceptDrops(true);
	ui->horizontalLayout_2->addWidget(view);


	if (simulacao->isSimulacaoEmHardware())
		statusTipoSimulacao->setText(tr("Simulação em Hardware"));
	else
		statusTipoSimulacao->setText(tr("Simulação em Software"));		

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
		dlg->setDoubleMinimum(0.01);
		dlg->setDoubleMaximum(1);
		dlg->setLabelText(tr("Razão de aspecto:"));
		dlg->setDoubleValue(Parametros::getInstance()->getRazaoAspectoDoPrismaTriangularTruncado());
		res = dlg->exec();

		if (res == QInputDialog::DialogCode::Accepted){
			Parametros::getInstance()->setRazaoAspectoDoPrismaTriangularTruncado(dlg->doubleValue());

			dlg->setDoubleMinimum(0.01);
			dlg->setDoubleMaximum(0.5);
			dlg->setLabelText(tr("Razão de Truncamento:"));
			dlg->setDoubleValue(Parametros::getInstance()->getRazaoDeTruncamentoDoPrismaTriangularTruncado());
			res = dlg->exec();

			if (res == QInputDialog::DialogCode::Accepted){
				Parametros::getInstance()->setRazaoDeTruncamentoDoPrismaTriangularTruncado(dlg->doubleValue());

				dlg->setDoubleMinimum(0);
				dlg->setDoubleMaximum(2147483647);
				dlg->setLabelText(tr("L0:"));
				dlg->setDoubleValue(Parametros::getInstance()->getL0DoPrismaTriangularTruncado());
				res = dlg->exec();
				if (res == QInputDialog::DialogCode::Accepted)
					Parametros::getInstance()->setL0DoPrismaTriangularTruncado(dlg->doubleValue());
			}
		}
		break;
	}
}
void MainWindow::adicionarObjetos(){

	long qtde =  ui->textNumeroGraos->text().toLong();
	Cor corDoGrao = VERMELHO;

	if (ui->checkBoxSolicitarCor->isChecked()){
		QColorDialog dialog;
		QColor cor = dialog.getColor(Qt::green,this,"Selecione a cor dos grãos a serem adicionados");
		if (cor.isValid()){
			corDoGrao.r = cor.red()/255.0f;
			corDoGrao.g = cor.green()/255.0f;
			corDoGrao.b = cor.blue()/255.0f;
		}else{
			return; // aborta a adição dos objetos
		}	
	}

	switch(ui->comboBoxTipoGrao->currentIndex())
	{
	case 0:
		simulacao->adicionarEsferas(qtde,corDoGrao);
		break;
	case 1:
		simulacao->adicionarPrismas(qtde,corDoGrao);
		break;
	case 2:
		simulacao->adicionarPrismasTruncados(qtde,corDoGrao);
		break;
	}	

	atualizarQuantidadeDeGraosEmCena();	

}


void MainWindow::pararSimulacao(){
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

void MainWindow::mudarEstrategiaDeAlturasDoPlanodeCorte(bool b){
	if (b)
	{
		int res;
		QInputDialog *dlg = new QInputDialog(this);
		dlg->setIntMinimum(1);
		dlg->setIntMaximum(INT_MAX);
		dlg->setInputMode(QInputDialog::InputMode::IntInput);

		dlg->setLabelText(tr("Quantidade de planos:"));
		res = dlg->exec();
		if (res == QInputDialog::DialogCode::Accepted){
			int qtdePlanos = dlg->intValue();
			Parametros *p = Parametros::getInstance();
			double h0 = p->getAlturaDaBaseDaCaixa();
			double h1 = p->getArestaDaCaixa() + p->getAlturaDaBaseDaCaixa();
			simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy(h0,h1,qtdePlanos));
			simulacao->novoPlanoDeCorte();
		}
		else
			this->ui->checkBoxPlanoDeCorteSistematico->setChecked(false);
	}
	else
		simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy);

}
void MainWindow::novoPlanoDeCorte(){
	simulacao->novoPlanoDeCorte();
}

void MainWindow::exibirSobre(){
	QMessageBox::warning(this, tr("teste"),tr("exibirSobre"));
}

void MainWindow::exibirPlanoDeCorte(bool b){
	if(b){
		simulacao->exibirPlanoDeCorte();
		ui->btnNovoPlanodeCorte->setEnabled(true);
		ui->btnPlanoDeCorteCor->setEnabled(true);
		ui->btnPlanovsGraos->setEnabled(true);
	}
	else{
		simulacao->esconderPlanoDeCorte();
		ui->btnNovoPlanodeCorte->setEnabled(false);
		ui->btnPlanoDeCorteCor->setEnabled(false);
		ui->btnPlanovsGraos->setEnabled(false);
	}
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

	ui->btnParar->setEnabled(true);
	ui->btnLimpar->setEnabled(true);
	ui->btnAdicionar->setEnabled(true);
	ui->btnParar->setText("Pausar");
	ui->btnNovoPlanodeCorte->setEnabled(true);
	ui->btnPlanovsGraos->setEnabled(true);
	ui->btnExibirInterceptos->setEnabled(false);
	ui->actionGrade_de_pontos_e_linha_teste->setEnabled(true);
	ui->btnAdicionar->setEnabled(true);

	ui->checkBoxExibirPlanoDeCorte->setChecked(true);
	ui->checkBoxExibirPontosTeste->setChecked(true);
	ui->checkBoxExibirRetasTeste->setChecked(true);

	Parametros::getInstance()->loadDefaultValues();
	criarCanvas();
	atualizarQuantidadeDeGraosEmCena();

}

void MainWindow::usarGraosAleatorios(bool b){
	GradeParams params = Parametros::getInstance()->getParametrosDaGrade();
	params.pontosAleatorios = b;
}

/** Exibe os grãos que estão sendo interceptados pelo plano */
void MainWindow::exibirGraosInterceptados(){

	simulacao->selecionarGraosInterceptados();
	atualizarQuantidadeDeGraosEmCena();

	ui->btnNovoPlanodeCorte->setEnabled(false);
	ui->btnParar->setEnabled(false);
	ui->btnLimpar->setEnabled(false);
	ui->btnAdicionar->setEnabled(false);
	ui->btnExibirInterceptos->setEnabled(true);
	ui->actionGrade_de_pontos_e_linha_teste->setEnabled(false);
	ui->btnPlanovsGraos->setEnabled(false);
}

/** Exibe os interceptos de seção dos objetos interceptados pelo plano */
void MainWindow::exibirInterceptos(){
	Grade *g = simulacao->getGrade();

	RenderizarInterceptosStrategy *strategy = new RenderizarInterceptosStrategy(g);
	view->setStrategy(strategy);

	this->view->habilitarMudancaDeEstrategiaDeVisualizacao(true);
	this->view->posicionarCameraNoTopoDaCaixa();
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