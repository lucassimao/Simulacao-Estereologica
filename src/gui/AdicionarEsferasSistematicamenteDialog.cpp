#include <QDialog>
#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include "ColorListEditor.h"
#include "TextBoxDelegate.h"
#include "AdicionarEsferasSistematicamenteDialog.h"
#include "..\model\SimulacaoCaixa.h"

using namespace simulacao::gui;
using namespace simulacao::model;

AdicionarEsferasSistematicamenteDialog::AdicionarEsferasSistematicamenteDialog(QWidget *parent, SimulacaoCaixa *simulacao):QDialog(parent){
	ui = new Ui_AdicionarEsferasSistematicamenteDialog();
	ui->setupUi(this);
	this->command = 0;
	this->simulacao = simulacao;
	this->raise();

	QDoubleValidator *valPercentual = new QDoubleValidator(this);
	valPercentual->setBottom(0);
	valPercentual->setDecimals(3);
	valPercentual->setTop(100);

	ui->textFaseSolida->setValidator(valPercentual);
	
	
	QItemEditorFactory *factory = new QItemEditorFactory;

    QItemEditorCreatorBase *colorListCreator =
         new QStandardItemEditorCreator<ColorListEditor>();

     factory->registerEditor(QVariant::Color, colorListCreator);
     QItemEditorFactory::setDefaultFactory(factory);

	model = new QStandardItemModel(0,3,this);
 
	model->setHeaderData( 0, Qt::Horizontal, QObject::tr("Raio") );
	model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Cor") );
	model->setHeaderData( 2, Qt::Horizontal, QObject::tr("Porcentagem") );
	  
	ui->tableEspecificacao->setModel(model);
    
	
	TextBoxDelegate *dlg = new TextBoxDelegate(valPercentual);
	ui->tableEspecificacao->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableEspecificacao->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableEspecificacao->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->tableEspecificacao->setItemDelegateForColumn(0,dlg);
    ui->tableEspecificacao->setItemDelegateForColumn(2,dlg);
	ui->tableEspecificacao->setColumnWidth(1,160);
	 
}

void AdicionarEsferasSistematicamenteDialog::adicionarDescricao(){

	int row = model->rowCount();
	model->insertRow(row);


	QModelIndex cell1 = model->index(row,0);
	QModelIndex cell2 = model->index(row,1);
	QModelIndex cell3 = model->index(row,2);

	model->setData(cell1,QVariant(0.0));
	model->setData(cell2,QColor("red"));
	model->setData(cell3,QVariant(0.0));

}

void AdicionarEsferasSistematicamenteDialog::removerDescricao(){
	QItemSelectionModel  *selectionModel = ui->tableEspecificacao->selectionModel();
	QModelIndexList list = selectionModel->selectedRows();

	if (list.size()==0)
	{
		QMessageBox *msg = new QMessageBox(QMessageBox::Information,tr("Informação"),
			tr("Selecione primeiro a linha na tabela que deseja excluir"),QMessageBox::Ok,this);
		msg->show();
	}
	else{
		// como apenas um item estara selecionado devido configurações anteriores ...
		QModelIndex index = list.at(0);
		this->model->removeRow(index.row());		
		
	}
}
void AdicionarEsferasSistematicamenteDialog::sair(){
	this->reject();
}



void AdicionarEsferasSistematicamenteDialog::adicionarEsferas(){
	
	bool valorValido  = false;
	double porcentagemFaseSolida = ui->textFaseSolida->text().toDouble(&valorValido);
	
	if (valorValido){
		int linhas = this->model->rowCount();
		command = new AdicionarObjetosCommand(this->simulacao,porcentagemFaseSolida);

		for(int row=0;row<linhas;++row){
			QModelIndex colunaRaio = this->model->index(row,0);
			QModelIndex colunaCor = this->model->index(row,1);
			QModelIndex colunaPorcentagem = this->model->index(row,2);

			double raio = this->model->data(colunaRaio, Qt::DisplayRole).toDouble();
			QColor cor = qVariantValue<QColor>(this->model->data(colunaCor, Qt::DisplayRole));
			double porcentagem = this->model->data(colunaPorcentagem, Qt::DisplayRole).toDouble();
			Cor c = {cor.red()/255.0f,cor.green()/255.0f,cor.blue()/255.0f};
			
			command->adicionarEsferas(raio,porcentagem,c);
		}
		this->accept();
	}
	else
	{
		QMessageBox *msg = new QMessageBox(QMessageBox::Information,tr("Informação"),
			tr("Forneça a porcentagem da fase sólida!"),QMessageBox::Ok,this);
		ui->textFaseSolida->setFocus();
		msg->show();
	}
}