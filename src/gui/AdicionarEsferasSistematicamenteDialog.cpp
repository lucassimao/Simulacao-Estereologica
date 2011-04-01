#include <QDialog>
#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include "ColorListEditor.h"
#include "TextBoxDelegate.h"
#include "AdicionarEsferasSistematicamenteDialog.h"
#include "..\model\SimulacaoCaixa.h"
#include "..\model\atores\Esfera.h"

using namespace simulacao::gui;
using namespace simulacao::model;
using namespace simulacao::model::atores;

AdicionarEsferasSistematicamenteDialog::AdicionarEsferasSistematicamenteDialog(QWidget *parent, SimulacaoCaixa *simulacao):QDialog(parent){
	ui = new Ui_AdicionarEsferasSistematicamenteDialog();
	ui->setupUi(this);
	this->command = 0;
	this->simulacao = simulacao;
	this->raise();

	QIntValidator *valQuantidade = new QIntValidator(this);
	valQuantidade->setBottom(0);

	QDoubleValidator *valPercentual = new QDoubleValidator(this);
	valPercentual->setBottom(0);
	valPercentual->setDecimals(10);
	valPercentual->setTop(100);

	ui->textFaseSolida->setValidator(valPercentual);
	ui->textFaseSolida->setText("50");
	QItemEditorFactory *factory = new QItemEditorFactory;

    QItemEditorCreatorBase *colorListCreator =  new QStandardItemEditorCreator<ColorListEditor>();

    factory->registerEditor(QVariant::Color, colorListCreator);
    QItemEditorFactory::setDefaultFactory(factory);

	model = new QStandardItemModel(0,4,this);
 
	model->setHeaderData( COLUNA_RAIO, Qt::Horizontal, QObject::tr("Raio") );
	model->setHeaderData( COLUNA_PORCENTAGEM, Qt::Horizontal, QObject::tr("Porcentagem") );
	model->setHeaderData( COLUNA_QUANTIDADE, Qt::Horizontal, QObject::tr("Quantidade") );
	model->setHeaderData( COLUNA_COR, Qt::Horizontal, QObject::tr("Cor") );

	ui->tableEspecificacao->setModel(model);	
	
	ui->tableEspecificacao->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableEspecificacao->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableEspecificacao->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	TextBoxDelegate *textBox = new TextBoxDelegate(valPercentual);
	TextBoxDelegate *textBox2 = new TextBoxDelegate(valQuantidade);

	ui->tableEspecificacao->setItemDelegateForColumn(0,textBox);
    ui->tableEspecificacao->setItemDelegateForColumn(1,textBox);
    ui->tableEspecificacao->setItemDelegateForColumn(2,textBox2);

	ui->tableEspecificacao->setColumnWidth(3,160);

	connect(model,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(manterProporcaoEntrePorcentagemEQuantidade(QStandardItem *))); 
	connect(ui->textFaseSolida,SIGNAL(textChanged (const QString &)),this,SLOT(manterProporcaoEntrePorcentagemEQuantidade())); 	 
}

void AdicionarEsferasSistematicamenteDialog::manterProporcaoEntrePorcentagemEQuantidade(){
	QStandardItem *item;
	for(int i=0;i< this->model->rowCount();++i)
	{
		item = this->model->item(i,COLUNA_PORCENTAGEM);
		manterProporcaoEntrePorcentagemEQuantidade(item);
	}
}

void AdicionarEsferasSistematicamenteDialog::manterProporcaoEntrePorcentagemEQuantidade(QStandardItem *item){
	int row = item->row();
	QModelIndex colunaPorcentagem = this->model->index(row,COLUNA_PORCENTAGEM);
	QModelIndex colunaQtde = this->model->index(row,COLUNA_QUANTIDADE);

	double porcentagem = this->model->data(colunaPorcentagem, Qt::DisplayRole).toDouble();
	double porcentagemFaseSolida = getPorcentagemFaseSolida();

	double raio = getRaio(row);
	double volumeTotalDaCaixa = this->simulacao->getVolumeDaCaixa();
	double volumeDaFaseSolida = (volumeTotalDaCaixa * porcentagemFaseSolida)/100.0;
	double volumeDeUmaEsfera = Esfera::calcularVolume(raio);

	this->model->blockSignals(true);

	switch(item->column()){
		case COLUNA_QUANTIDADE:
			{
				int qtde = this->model->data(colunaQtde, Qt::DisplayRole).toInt();
				double volumeTotalDasEsferas =  qtde*volumeDeUmaEsfera;
				double porcentagemDaFaseSolida = (100.0 * volumeTotalDasEsferas)/volumeDaFaseSolida;
				
				if (porcentagemDaFaseSolida>0)	this->model->setData(colunaPorcentagem,QVariant(porcentagemDaFaseSolida));
			}
			break;
		default:
			{
				double volumeTotalDasEsferas = (porcentagem*volumeDaFaseSolida)/100.0;				
				if (volumeDeUmaEsfera>0){
					int qtde = volumeTotalDasEsferas/volumeDeUmaEsfera;
					this->model->setData(colunaQtde,QVariant(qtde));
				}
			}
			break;
	}

	this->model->blockSignals(false);	
}

double AdicionarEsferasSistematicamenteDialog::getPorcentagemFaseSolida(){
	bool valorValido  = false;
	double porcentagemFaseSolida = ui->textFaseSolida->text().toDouble(&valorValido);
	if (valorValido)
		return porcentagemFaseSolida;
	else return 0;
}

double AdicionarEsferasSistematicamenteDialog::getRaio(int linha){
	QModelIndex coluna = this->model->index(linha,COLUNA_RAIO);
	return this->model->data(coluna, Qt::DisplayRole).toDouble();
}

void AdicionarEsferasSistematicamenteDialog::adicionarDescricao(){

	int row = model->rowCount();
	model->insertRow(row);


	QModelIndex cell1 = model->index(row,COLUNA_RAIO);
	QModelIndex cell2 = model->index(row,COLUNA_PORCENTAGEM);
	QModelIndex cell3 = model->index(row,COLUNA_QUANTIDADE);
	QModelIndex cell4 = model->index(row,COLUNA_COR);

	model->setData(cell1,QVariant(0.0));
	model->setData(cell2,QVariant(0.0));
	model->setData(cell3,QVariant(0.0));
	model->setData(cell4,QColor("red"));

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
			QModelIndex colunaRaio = this->model->index(row,COLUNA_RAIO);
			QModelIndex colunaCor = this->model->index(row,COLUNA_COR);
			QModelIndex colunaPorcentagem = this->model->index(row,COLUNA_PORCENTAGEM);

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