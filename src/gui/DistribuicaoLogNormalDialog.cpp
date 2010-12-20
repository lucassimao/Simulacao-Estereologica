#include <QDialog>
#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include <stdexcept>
#include "..\model\Parametros.h"
#include "ColorListEditor.h"
#include "TextBoxDelegate.h"
#include "DistribuicaoLogNormalDialog.h"
#include "..\model\SimulacaoCaixa.h"
#include "..\model\atores\PrismaTriangularTruncado.h"
#define _USE_MATH_DEFINES
#include <cmath>

using namespace simulacao::gui;
using namespace simulacao::model;
using namespace simulacao::model::atores;

DistribuicaoLogNormalDialog::DistribuicaoLogNormalDialog(QWidget *parent, SimulacaoCaixa *simulacao):QDialog(parent){
	ui = new Ui_DistribuicaoLogNormalDialog();
	ui->setupUi(this);
	this->command = NULL;
	this->simulacao = simulacao;
	this->raise();
	
	configurarValidadoresDosCamposDeTexto();
	configurarEditorDeCores();
	configurarModeloDaTabela();
	configurarCamadaDeViewDaTabela();
	configurarOrdemDoTab();
}

void DistribuicaoLogNormalDialog::configurarOrdemDoTab(){
	setTabOrder(ui->textX0,ui->textXn);
	setTabOrder(ui->textXn,ui->textDeltaX);
	setTabOrder(ui->textDeltaX,ui->textRazaoDeAspecto);
	setTabOrder(ui->textRazaoDeAspecto,ui->textRazaoDeTruncamento);
	setTabOrder(ui->textRazaoDeTruncamento,ui->textFracaoDeVazio);
	setTabOrder(ui->textFracaoDeVazio,ui->textSigma);
	setTabOrder(ui->textSigma,ui->textMi);
	setTabOrder(ui->textMi,ui->textN0);
}

void DistribuicaoLogNormalDialog::sair(){
	this->reject();
}

void DistribuicaoLogNormalDialog::adicionarPrismas(){
	bool valorValido;
	double fracaoVazia = ui->textFracaoDeVazio->text().toDouble(&valorValido);
	double porcentagemFaseSolida =0;
	if (valorValido)
		porcentagemFaseSolida =  (100 - fracaoVazia)/100.0;
	else
		porcentagemFaseSolida = 0;

	int linhas = this->model->rowCount();
	this->command = new AdicionarObjetosCommand(this->simulacao,porcentagemFaseSolida);
	double volumeTotal = 0;
	double razaoDeAspecto = getRazaoDeAspecto();
	double razaoDeTruncamento = getRazaoDeTruncamento();

	for(int row = 0; row < linhas; ++row){
		QModelIndex cell1 = model->index(row,COLUNA_X);
		QModelIndex cell2 = model->index(row,COLUNA_L0);
		QModelIndex cell3 = model->index(row,COLUNA_IMAGEM_NAO_NORMALIZADA);
		QModelIndex cell4 = model->index(row,COLUNA_IMAGEM_NORMALIZADA);
		QModelIndex cell5 = model->index(row,COLUNA_QUANTIDADE);

		double L0 = this->model->data(cell2, Qt::DisplayRole).toDouble();
		int quantidade = this->model->data(cell5, Qt::DisplayRole).toInt();

		double volumeDoPrisma = PrismaTriangularTruncado::calcularVolume(razaoDeAspecto,razaoDeTruncamento,L0);

		volumeTotal += volumeDoPrisma*quantidade;

		Cor c = {1.0f,1.0f,1.0f};
		
		command->adicionarPrismas(L0,quantidade,c,razaoDeAspecto,razaoDeTruncamento);
	}
	double ladoDaCaixa = pow(volumeTotal/porcentagemFaseSolida,1/3.0);
	Parametros::getInstance()->setArestaDaCaixa(ladoDaCaixa);
	this->simulacao->criarCaixa();
	this->accept();
}

void DistribuicaoLogNormalDialog::configurarEditorDeCores(){
	QItemEditorFactory *factory = new QItemEditorFactory;
    QItemEditorCreatorBase *colorListCreator = new QStandardItemEditorCreator<ColorListEditor>();
	factory->registerEditor(QVariant::Color, colorListCreator);
    QItemEditorFactory::setDefaultFactory(factory);
}

void DistribuicaoLogNormalDialog::configurarModeloDaTabela(){
	model = new QStandardItemModel(0,5,this);

	model->setHeaderData( COLUNA_X, Qt::Horizontal, QObject::tr("X") );
	model->setHeaderData( COLUNA_L0, Qt::Horizontal, QObject::tr("L0") );
	model->setHeaderData( COLUNA_IMAGEM_NAO_NORMALIZADA, Qt::Horizontal,  QObject::tr("f(x,o,u) não normalizada"));
	model->setHeaderData( COLUNA_IMAGEM_NORMALIZADA, Qt::Horizontal, QObject::tr("f(x,o,u) normalizada") );
	model->setHeaderData( COLUNA_QUANTIDADE, Qt::Horizontal, QObject::tr("Quantidade") );
	ui->tableDistribuicao->setModel(model);
}

void DistribuicaoLogNormalDialog::configurarCamadaDeViewDaTabela(){
	ui->tableDistribuicao->setColumnWidth(COLUNA_IMAGEM_NORMALIZADA,150);
	ui->tableDistribuicao->setColumnWidth(COLUNA_IMAGEM_NAO_NORMALIZADA,150);
}

void DistribuicaoLogNormalDialog::configurarValidadoresDosCamposDeTexto(){
	QIntValidator *intValidator = new QIntValidator(this);
	intValidator->setBottom(0);

	ui->textDeltaX->setValidator(intValidator);
	ui->textXn->setValidator(intValidator);
	ui->textX0->setValidator(intValidator);
	ui->textN0->setValidator(intValidator);

	QDoubleValidator *fracaoDeVazioValidator = new QDoubleValidator(this) ;
	fracaoDeVazioValidator->setRange(0,100,2);
	ui->textFracaoDeVazio->setValidator(fracaoDeVazioValidator);

	QDoubleValidator *razaoDeAspectoValidator = new QDoubleValidator(this);
	razaoDeAspectoValidator->setRange(0.001,1,3);
	ui->textRazaoDeAspecto->setValidator(razaoDeAspectoValidator);

	QDoubleValidator *razaoDeTruncamentoValidator = new QDoubleValidator(this);
	razaoDeTruncamentoValidator->setRange(0,0.5,3);
	ui->textRazaoDeTruncamento->setValidator(razaoDeTruncamentoValidator);

	QDoubleValidator *sigmaMiValidator = new QDoubleValidator(this);
	razaoDeTruncamentoValidator->setRange(0,100,2);
	ui->textSigma->setValidator(sigmaMiValidator);
	ui->textMi->setValidator(sigmaMiValidator);
}

int DistribuicaoLogNormalDialog::getX0(){ return ui->textX0->text().toInt(); }
int DistribuicaoLogNormalDialog::getXn(){ return ui->textXn->text().toInt(); }
double DistribuicaoLogNormalDialog::getMi(){ return ui->textMi->text().toDouble(); }
double DistribuicaoLogNormalDialog::getSigma(){ return ui->textSigma->text().toDouble(); }
double DistribuicaoLogNormalDialog::getRazaoDeAspecto(){ return ui->textRazaoDeAspecto->text().toDouble(); }
double DistribuicaoLogNormalDialog::getRazaoDeTruncamento(){ return ui->textRazaoDeTruncamento->text().toDouble(); }
double DistribuicaoLogNormalDialog::getN0(){ return ui->textN0->text().toDouble(); }
float DistribuicaoLogNormalDialog::getDeltaX(){ return ui->textDeltaX->text().toFloat(); }

void DistribuicaoLogNormalDialog::criarDistribuicaoDeGraos(){
	int x0 = getX0();
	int xn = getXn();
	float deltaX = getDeltaX();
	double mi = getMi();
	double sigma = getSigma();
	double n0 = getN0();
	

	model->removeRows(0, model->rowCount());
	double somaDaImagemDaFuncaoLogNormalNaoNormalizado = 0;

	for(int i=0; x0 + deltaX*i <= xn; ++i){
		int row = model->rowCount();
		model->insertRow(row);

		QModelIndex cell1 = model->index(row,COLUNA_X);
		QModelIndex cell2 = model->index(row,COLUNA_L0);
		QModelIndex cell3 = model->index(row,COLUNA_IMAGEM_NAO_NORMALIZADA);
		QModelIndex cell4 = model->index(row,COLUNA_IMAGEM_NORMALIZADA);
		QModelIndex cell5 = model->index(row,COLUNA_QUANTIDADE);

		double xi = x0 + deltaX*i;
		model->setData(cell1,QVariant(xi));
		model->setData(cell2,QVariant(0.0));
		double logNormal = calcularDistribuicaoLogNormal(xi,mi,sigma);
		somaDaImagemDaFuncaoLogNormalNaoNormalizado += logNormal;
		model->setData(cell3,QVariant(logNormal));
		model->setData(cell4,QVariant(0.0));
		model->setData(cell5,QVariant(0.0));
	}	

	//normalizando a imagem da função
	for(int row=0; row<model->rowCount(); ++row){
		QModelIndex cell1 = model->index(row,COLUNA_IMAGEM_NAO_NORMALIZADA);
		QModelIndex cell2 = model->index(row,COLUNA_IMAGEM_NORMALIZADA);
		QModelIndex cell5 = model->index(row,COLUNA_QUANTIDADE);
		
		double imagemNaoNormalizada = this->model->data(cell1, Qt::DisplayRole).toDouble();
		double imagemNormalizada = imagemNaoNormalizada/somaDaImagemDaFuncaoLogNormalNaoNormalizado;
		int quantidade = n0 * imagemNormalizada;
		this->model->setData(cell2,QVariant(imagemNormalizada));
		this->model->setData(cell5,QVariant(quantidade));
	}

	calcularL0s();

}

double DistribuicaoLogNormalDialog::calcularDistribuicaoLogNormal(double x, double mi, double sigma){
	if(x==0 || sigma==0)
		return 0;
	else
		return (1.0/x*sigma*sqrt(2*M_PI)) *  exp(-0.5*pow((log(x) - mi)/sigma,2));
}


void DistribuicaoLogNormalDialog::calcularL0s(){
	double alpha = getRazaoDeAspecto();
	double beta = getRazaoDeTruncamento();
	double raizQuadraDe3 = sqrt(3.0);
	double volumeFaseSolida = 0;

	if (this->ui->radioButtonUsarXComoVolume->isChecked()){
		for(int row=0; row < model->rowCount(); ++row){			
			QModelIndex cellX = model->index(row,COLUNA_X);
			QModelIndex cellL0 = model->index(row,COLUNA_L0);
			QModelIndex cellQuantidade = model->index(row,COLUNA_QUANTIDADE);

			double x = this->model->data(cellX, Qt::DisplayRole).toDouble();
			int quantidade = this->model->data(cellQuantidade, Qt::DisplayRole).toInt();
			double l0 = pow(  x/( (raizQuadraDe3/4.0)*alpha*(1 - 3*pow(beta,2)) ) , 1/3.0);
			volumeFaseSolida += quantidade * PrismaTriangularTruncado::calcularVolume(alpha,beta,l0);
			this->model->setData(cellL0,QVariant(l0));
		}
	}else{
		for(int row=0; row < model->rowCount(); ++row){			
			QModelIndex cellX = model->index(row,COLUNA_X);
			QModelIndex cellL0 = model->index(row,COLUNA_L0);
			QModelIndex cellQuantidade = model->index(row,COLUNA_QUANTIDADE);

			double x = this->model->data(cellX, Qt::DisplayRole).toDouble();
			int quantidade = this->model->data(cellQuantidade, Qt::DisplayRole).toInt();
			double l0 = pow( pow(x,3)/( (3*raizQuadraDe3/(2.0*M_PI))*alpha*(1 -3*pow(beta,2)) ),1/3.0);
			volumeFaseSolida += quantidade * PrismaTriangularTruncado::calcularVolume(alpha,beta,l0);
			this->model->setData(cellL0,QVariant(l0));
		}
	}
	ui->labelVolumedaFaseSolida->setText(tr("%1").arg(volumeFaseSolida));
	bool valorValido;
	double fracaoVazia = ui->textFracaoDeVazio->text().toDouble(&valorValido);
	double porcentagemFaseSolida =0;
	if (valorValido)
		porcentagemFaseSolida =  (100 - fracaoVazia)/100.0;
	else
		porcentagemFaseSolida = 0;
	double ladoDaCaixa = pow(volumeFaseSolida/porcentagemFaseSolida,1/3.0);
	ui->labelLadoFinalDaCaixa->setText(tr("%1").arg(ladoDaCaixa));

}