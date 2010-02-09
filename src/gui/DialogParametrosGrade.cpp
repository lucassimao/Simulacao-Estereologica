#include "DialogParametrosGrade.h"
#include "..\model\Parametros.h"
#include "..\model\grade\Grade.h"

using namespace simulacao::model;
using namespace simulacao::gui;
using namespace simulacao::model::grade;

DialogParametrosGrade::DialogParametrosGrade(QWidget *parent):QDialog(parent){

	ui = new Ui_DlgParametrosDaGrade();
	ui->setupUi(this);
	this->raise();
	ui->textQtdeLinhas->setValidator(new QIntValidator(0,1000,this));
	ui->textQtdeLinhas->setText(tr("%1").arg(Parametros::getInstance()->getParametrosDaGrade().qtdeLinhas));

	ui->textQtdePontosLinha->setValidator(new QIntValidator(0,1000,this));
	ui->textQtdePontosLinha->setText(tr("%1").arg(Parametros::getInstance()->getParametrosDaGrade().qtdePontos));
	
	ui->textDistanciaEntreLinhas->setValidator(new QDoubleValidator(0,Parametros::getInstance()->getArestaDaCaixa(),2, this));	
	ui->textDistanciaEntrePontos->setValidator(new QDoubleValidator(0,Parametros::getInstance()->getArestaDaCaixa(),2, this));
	
	recalcularDistancias();	
}

DialogParametrosGrade::~DialogParametrosGrade(){
}

void DialogParametrosGrade::salvarAlteracoes(){
	Parametros::getInstance()->setParametrosDaGrade(ui->textQtdeLinhas->text().toInt(), 
													ui->textQtdePontosLinha->text().toInt(),
													ui->checkBoxUsarPontosAleatorios->checkState() == Qt::CheckState::Checked);
	this->close();
}
void DialogParametrosGrade::cancelarAlteracoes(){
	this->close();
}

void DialogParametrosGrade::recalcularQuantidades(){
	int linhasOut=0, pontosOut=0;
	double distanciaEntrePontos = ui->textDistanciaEntrePontos->text().toDouble();
	double distanciaEntreLinhas = ui->textDistanciaEntreLinhas->text().toDouble();

	if (distanciaEntreLinhas>0 && distanciaEntrePontos>0){
		Grade::calcularQtdeDeLinhasEPontos(distanciaEntrePontos,distanciaEntreLinhas,&linhasOut,&pontosOut);
		ui->textQtdeLinhas->setText(tr("%1").arg(linhasOut));
		ui->textQtdePontosLinha->setText(tr("%1").arg(pontosOut));
	}
}
void DialogParametrosGrade::recalcularDistancias(){

	int qtdePontos = ui->textQtdePontosLinha->text().toDouble();
	int qtdeLinhas = ui->textQtdeLinhas->text().toDouble();
	double distanciaEntrePontos=0,distanciaEntreLinhas=0;

	if (qtdePontos>0 && qtdeLinhas>0){
		Grade::calcularDistanciaEntreLinhasEDistanciaEntrePontos(qtdeLinhas,qtdePontos,&distanciaEntrePontos,&distanciaEntreLinhas);
		ui->textDistanciaEntreLinhas->setText(tr("%1").arg(distanciaEntreLinhas));
		ui->textDistanciaEntrePontos->setText(tr("%1").arg(distanciaEntrePontos));
	}
}