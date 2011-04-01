#include <QDialog>
#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include <stdexcept>
#include "..\model\Parametros.h"
#include "ColorListEditor.h"
#include "TextBoxDelegate.h"
#include "AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog.h"
#include "..\model\SimulacaoCaixa.h"
#include "..\model\atores\Esfera.h"

using namespace simulacao::gui;
using namespace simulacao::model;
using namespace simulacao::model::atores;

AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog(QWidget *parent, SimulacaoCaixa *simulacao):QDialog(parent){
	ui = new Ui_AdicionarEsferasSistematicamenteComAjusteDaCaixaDialog();
	ui->setupUi(this);
	this->command = 0;
	this->simulacao = simulacao;
	this->raise();

	QDoubleValidator *valPercentual = new QDoubleValidator(this);
	valPercentual->setBottom(0);
	valPercentual->setDecimals(3);
	valPercentual->setTop(100);

	QIntValidator *valQuantidade = new QIntValidator(this);
	valPercentual->setBottom(0);

	ui->textFracaoVazia->setValidator(valPercentual);
	ui->textFracaoVazia->setText("50");
	
	configurarEditorDeCores();
	configurarModeloDaTabela();
	configurarCamadaDeViewDaTabela(valPercentual,valQuantidade);

	connect(ui->textFracaoVazia,SIGNAL(textChanged (const QString &)),this,SLOT(atualizarLabelLadoDaCaixa()));
	 
}

void AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::configurarEditorDeCores(){
    QItemEditorFactory *factory = new QItemEditorFactory;
    QItemEditorCreatorBase *colorListCreator = new QStandardItemEditorCreator<ColorListEditor>();
	factory->registerEditor(QVariant::Color, colorListCreator);
    QItemEditorFactory::setDefaultFactory(factory);
}

void AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::configurarModeloDaTabela(){
	model = new QStandardItemModel(0,3,this);
 
	model->setHeaderData( COLUNA_RAIO, Qt::Horizontal, QObject::tr("Raio") );
	model->setHeaderData( COLUNA_QUANTIDADE, Qt::Horizontal, QObject::tr("Quantidade") );
	model->setHeaderData( COLUNA_COR, Qt::Horizontal, QObject::tr("Cor") );

	ui->tableEspecificacao->setModel(model);
	connect(model,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(atualizarLabelVolumeFaseSolida())); 
	connect(model,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(atualizarLabelLadoDaCaixa())); 

}

void AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::configurarCamadaDeViewDaTabela(QDoubleValidator *valPercentual,QIntValidator *valQuantidade){
	TextBoxDelegate *texBox1 = new TextBoxDelegate(valPercentual);
	TextBoxDelegate *texBox2 = new TextBoxDelegate(valQuantidade);

	ui->tableEspecificacao->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableEspecificacao->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableEspecificacao->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	ui->tableEspecificacao->setItemDelegateForColumn(COLUNA_RAIO,texBox1); //valPercentual tb válido para especificar raio
	ui->tableEspecificacao->setItemDelegateForColumn(COLUNA_QUANTIDADE,texBox2);

	ui->tableEspecificacao->setColumnWidth(COLUNA_RAIO,100);
	ui->tableEspecificacao->setColumnWidth(COLUNA_QUANTIDADE,115);
	ui->tableEspecificacao->setColumnWidth(COLUNA_COR,160);
}


double AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::getPorcentagemFaseSolida(){
	bool valorValido  = false;
	double fracaoVazia = ui->textFracaoVazia->text().toDouble(&valorValido);
	if (valorValido)
		return 100 - fracaoVazia;
	else{
		throw exception("Forneça uma fração de vazio válida");	
	}
}

double AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::getRaio(int linha){
	QModelIndex coluna = this->model->index(linha, COLUNA_RAIO);
	return this->model->data(coluna, Qt::DisplayRole).toDouble();
}

int AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::getQuantidade(int linha){
	QModelIndex coluna = this->model->index(linha, COLUNA_QUANTIDADE);
	return this->model->data(coluna, Qt::DisplayRole).toInt();
}

QColor AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::getCor(int linha){
	QModelIndex coluna = this->model->index(linha, COLUNA_COR);
	return qVariantValue<QColor>(this->model->data(coluna, Qt::DisplayRole));
}

void AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::adicionarDescricao(){

	int row = model->rowCount();
	model->insertRow(row);

	QModelIndex cell1 = model->index(row,COLUNA_RAIO);
	QModelIndex cell2 = model->index(row,COLUNA_QUANTIDADE);
	QModelIndex cell3 = model->index(row,COLUNA_COR);

	model->setData(cell1,QVariant(0.0));
	model->setData(cell2,QVariant(0.0));
	model->setData(cell3,QColor("red"));
}

void AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::removerDescricao(){
	QItemSelectionModel  *selectionModel = ui->tableEspecificacao->selectionModel();
	QModelIndexList list = selectionModel->selectedRows();

	if (list.size()==0)	{
		QMessageBox *msg = new QMessageBox(QMessageBox::Information,tr("Informação"),
			tr("Selecione primeiro a linha na tabela que deseja excluir"),QMessageBox::Ok,this);
		msg->show();
	}
	else{
		assert(list.size() == 1);

		QModelIndex index = list.at(0);
		this->model->removeRow(index.row());
		atualizarLabelVolumeFaseSolida();		
	}
}

void AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::sair(){
	this->reject();
}

void AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::atualizarLabelVolumeFaseSolida(){
	this->ui->labelVolumeFaseSolida->setText(tr("%1").arg(getVolumeDaFaseSolida()));
}

void AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::atualizarLabelLadoDaCaixa(){
	try{
		double porcentagemFaseSolida = getPorcentagemFaseSolida()/100.0;
		double ladoDaCaixa =  pow(getVolumeDaFaseSolida()/porcentagemFaseSolida,1/3.0);
		this->ui->labelLadoDaCaixa->setText(tr("%1").arg(ladoDaCaixa));
	}catch(exception &e){
		QMessageBox *msg = new QMessageBox(QMessageBox::Information,tr("Atenção"),tr(e.what()),QMessageBox::Ok,this);
		msg->show();
	}
}

double AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::getVolumeDaFaseSolida(){
	int linhas = this->model->rowCount();
	double volumeTotal = 0;

	for(int row = 0; row < linhas; ++row){
		double raio = getRaio(row);
		int quantidade = getQuantidade(row);
		double volumeDaEsfera = Esfera::calcularVolume(raio);
		volumeTotal += volumeDaEsfera*quantidade;		
	}
	return volumeTotal;

}

void AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog::adicionarEsferas(){
	try{
		double porcentagemFaseSolida = getPorcentagemFaseSolida()/100.0;

		int linhas = this->model->rowCount();
		this->command = new AdicionarObjetosCommand(this->simulacao,porcentagemFaseSolida);
		double volumeTotal = 0;

		for(int row = 0; row < linhas; ++row){
			double raio = getRaio(row);
			int quantidade = getQuantidade(row);
			QColor cor = getCor(row);
			Cor c = {cor.red()/255.0f,cor.green()/255.0f,cor.blue()/255.0f};

			double volumeDaEsfera = Esfera::calcularVolume(raio);
			volumeTotal += volumeDaEsfera*quantidade;			
			command->adicionarEsferas(raio,quantidade,c);
		}
		double ladoDaCaixa = pow(volumeTotal/porcentagemFaseSolida,1/3.0);
		Parametros::getInstance()->setArestaDaCaixa(ladoDaCaixa);
		this->simulacao->criarCaixa();
		this->accept();
	}catch(exception &e){
		QMessageBox *msg = new QMessageBox(QMessageBox::Information,tr("Atenção"),tr(e.what()),QMessageBox::Ok,this);
		msg->show();
	}

}