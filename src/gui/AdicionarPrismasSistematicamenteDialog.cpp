#include <QDialog>
#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include "ColorListEditor.h"
#include "TextBoxDelegate.h"
#include "AdicionarPrismasSistematicamenteDialog.h"
#include "..\model\SimulacaoCaixa.h"

using namespace simulacao::gui;
using namespace simulacao::model;

AdicionarPrismasSistematicamenteDialog::AdicionarPrismasSistematicamenteDialog(QWidget *parent, SimulacaoCaixa *simulacao):QDialog(parent){
	ui = new Ui_AdicionarPrismasSistematicamenteDialog();
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

	model = new QStandardItemModel(0,5,this);
 
	model->setHeaderData( 0, Qt::Horizontal, QObject::tr("L0") );
	model->setHeaderData( 1, Qt::Horizontal, QObject::tr("Razão de Aspecto") );
	model->setHeaderData( 2, Qt::Horizontal, QObject::tr("Razão de truncamento") );
	model->setHeaderData( 3, Qt::Horizontal, QObject::tr("Porcentagem") );
	model->setHeaderData( 4, Qt::Horizontal, QObject::tr("Cor") );
	  
	ui->tableEspecificacao->setModel(model);
    
	
	TextBoxDelegate *dlg = new TextBoxDelegate(valPercentual);
	ui->tableEspecificacao->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableEspecificacao->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableEspecificacao->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->tableEspecificacao->setItemDelegateForColumn(0,dlg);
	ui->tableEspecificacao->setItemDelegateForColumn(1,dlg);
	ui->tableEspecificacao->setItemDelegateForColumn(2,dlg);
	ui->tableEspecificacao->setItemDelegateForColumn(3,dlg);
	ui->tableEspecificacao->setColumnWidth(0,75);
	ui->tableEspecificacao->setColumnWidth(1,110);
	ui->tableEspecificacao->setColumnWidth(2,150);
	ui->tableEspecificacao->setColumnWidth(4,160);
	 
}

void AdicionarPrismasSistematicamenteDialog::adicionarDescricao(){

	int row = model->rowCount();
	model->insertRow(row);


	QModelIndex cell1 = model->index(row,0);
	QModelIndex cell2 = model->index(row,1);
	QModelIndex cell3 = model->index(row,2);
	QModelIndex cell4 = model->index(row,3);
	QModelIndex cell5 = model->index(row,4);

	model->setData(cell1,QVariant(0.0));
	model->setData(cell2,QVariant(0.0));
	model->setData(cell3,QVariant(0.0));
	model->setData(cell4,QVariant(0.0));
	model->setData(cell5,QColor("red"));


}

void AdicionarPrismasSistematicamenteDialog::removerDescricao(){
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
void AdicionarPrismasSistematicamenteDialog::sair(){
	this->reject();
}



void AdicionarPrismasSistematicamenteDialog::adicionarEsferas(){
	
	bool valorValido  = false;
	double porcentagemFaseSolida = ui->textFaseSolida->text().toDouble(&valorValido);
	
	if (valorValido){
		int linhas = this->model->rowCount();
		command = new AdicionarObjetosCommand(this->simulacao,porcentagemFaseSolida);

		for(int row=0;row<linhas;++row){
			QModelIndex colunaL0 = this->model->index(row,0);
			QModelIndex colunaRazaoDeAspecto = this->model->index(row,1);
			QModelIndex colunaRazaoDeTruncamento = this->model->index(row,2);
			QModelIndex colunaPorcentagem = this->model->index(row,3);
			QModelIndex colunaCor = this->model->index(row,4);


			double l0 = this->model->data(colunaL0, Qt::DisplayRole).toDouble();
			double razaoDeAspecto = this->model->data(colunaRazaoDeAspecto, Qt::DisplayRole).toDouble();
			double razaoDeTruncamento = this->model->data(colunaRazaoDeTruncamento, Qt::DisplayRole).toDouble();
			double porcentagem = this->model->data(colunaPorcentagem, Qt::DisplayRole).toDouble();
			QColor cor = qVariantValue<QColor>(this->model->data(colunaCor, Qt::DisplayRole));

			Cor c = {cor.red()/255.0f,cor.green()/255.0f,cor.blue()/255.0f};
			
			command->adicionarPrismas(l0,porcentagem,c,razaoDeAspecto,razaoDeTruncamento);
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