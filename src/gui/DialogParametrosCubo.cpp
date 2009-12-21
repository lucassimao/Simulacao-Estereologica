#include "DialogParametrosCubo.h"

using namespace simulacao::gui;


DialogParametrosCubo::DialogParametrosCubo(QWidget *parent,SimulacaoCaixa *simulacao):QDialog(parent){
	ui = new Ui_WinParametrosCubo();
	ui->setupUi(this);
	this->raise();
	ui->textAresta->setValidator(new QDoubleValidator(this));
	this->simulacao = simulacao;
	ui->textAresta->setText(tr("%1").arg(this->simulacao->getArestaCubo()));
}

DialogParametrosCubo::~DialogParametrosCubo(){

}
void DialogParametrosCubo::salvarAresta(){
	this->simulacao->setArestaCubo(ui->textAresta->text().toDouble());
	this->close();
}


