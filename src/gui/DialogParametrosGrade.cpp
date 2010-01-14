#include "DialogParametrosGrade.h"

using namespace simulacao::gui;



DialogParametrosGrade::DialogParametrosGrade(QWidget *parent,SimulacaoCaixa *simulacao):QDialog(parent){
		ui = new Ui_DlgParametrosGradeDePontosELinhaTeste();
	ui->setupUi(this);
	this->raise();
	ui->textQtdeLinhas->setValidator(new QIntValidator(this));
	ui->textQtdePontosLinha->setValidator(new QIntValidator(this));

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