#include <QDialog>
#include <QtGui>
#include "ColorListEditor.h"
#include "TextBoxDelegate.h"
#include "AdicionarEsferasSistematicamenteDialog.h"

using namespace simulacao::gui;

AdicionarEsferasSistematicamenteDialog::AdicionarEsferasSistematicamenteDialog(QWidget *parent):QDialog(parent){
	ui = new Ui_AdicionarEsferasSistematicamenteDialog();
	ui->setupUi(this);
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
		this->model->removeRow(0);
		//mostrar msg ...

	}
	else{
		// como apenas um iten estara selecionado devido configurações anteriores ...
		QModelIndex index = list.at(0);
		this->model->removeRow(index.row());

		/*
      i = table->index(row, 1, QModelIndex());
         QVariant varAddr = table->data(i, Qt::DisplayRole);
         address = varAddr.toString();
		*/	
			
		
	}
}
void AdicionarEsferasSistematicamenteDialog::sair(){
	this->close();
}
void AdicionarEsferasSistematicamenteDialog::adicionarEsferas(){
	// ...
	this->close();

}