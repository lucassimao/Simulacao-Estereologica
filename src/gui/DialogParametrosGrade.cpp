#include "DialogParametrosGrade.h"
#include "..\model\Parametros.h"

using namespace simulacao::model;
using namespace simulacao::gui;



DialogParametrosGrade::DialogParametrosGrade(QWidget *parent,SimulacaoCaixa *simulacao):QDialog(parent){
		ui = new Ui_DlgParametrosDaGrade();
	ui->setupUi(this);
	this->raise();
	ui->textQtdeLinhas->setValidator(new QIntValidator(0,1000,this));
	ui->textQtdePontosLinha->setValidator(new QIntValidator(0,1000,this));
	ui->textDistanciaEntreLinhas->setValidator(new QDoubleValidator(0,Parametros::getInstance()->getArestaDaCaixa(),2, this));
	ui->textDistanciaEntrePontos->setValidator(new QDoubleValidator(0,Parametros::getInstance()->getArestaDaCaixa(),2, this));


	this->simulacao = simulacao;
	
}

DialogParametrosGrade::~DialogParametrosGrade(){
}

void DialogParametrosGrade::salvarAlteracoes(){
	
	this->close();
}
void DialogParametrosGrade::cancelarAlteracoes(){
	this->close();
}