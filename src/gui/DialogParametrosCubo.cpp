#include "DialogParametrosCubo.h"

using namespace simulacao::gui;


DialogParametrosCubo::DialogParametrosCubo(QWidget *parent):QDialog(parent){
	ui = new Ui_WinParametrosCubo();
	ui->setupUi(this);
	this->raise();
	ui->textAresta->setValidator(new QDoubleValidator(this));
}

DialogParametrosCubo::~DialogParametrosCubo(){

}
void DialogParametrosCubo::salvarAresta(){

	this->close();
}


