#include "DialogParametrosCubo.h"

using namespace simulacao::gui;


DialogParametrosCubo::DialogParametrosCubo(SimulacaoCaixa *simulacao){
	ui = new Ui_WinParametrosCubo();
	ui->setupUi(this);
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


