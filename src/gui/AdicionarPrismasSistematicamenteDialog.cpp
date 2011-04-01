#include <QDialog>
#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include <stdexcept>
#include "ColorListEditor.h"
#include "TextBoxDelegate.h"
#include "AdicionarPrismasSistematicamenteDialog.h"
#include "..\model\SimulacaoCaixa.h"
#include "..\model\atores\PrismaTriangularTruncado.h"

using namespace simulacao::gui;
using namespace simulacao::model;
using namespace simulacao::model::atores;
using std::runtime_error;

AdicionarPrismasSistematicamenteDialog::AdicionarPrismasSistematicamenteDialog(QWidget *parent, SimulacaoCaixa *simulacao):QDialog(parent){
	ui = new Ui_AdicionarPrismasSistematicamenteDialog();
	ui->setupUi(this);
	this->command = 0;
	this->simulacao = simulacao;
	this->raise();

	QDoubleValidator *valPercentual = new QDoubleValidator(this);
	valPercentual->setBottom(0);
	valPercentual->setDecimals(10);
	valPercentual->setTop(100);

	QIntValidator *valQuantidade = new QIntValidator(this);
	valPercentual->setBottom(0);

	ui->textFracaoVazia->setValidator(valPercentual);
	ui->textFracaoVazia->setText("50");

	QItemEditorFactory *factory = new QItemEditorFactory;

	QItemEditorCreatorBase *colorListCreator = new QStandardItemEditorCreator<ColorListEditor>();

	factory->registerEditor(QVariant::Color, colorListCreator);
	QItemEditorFactory::setDefaultFactory(factory);

	model = new QStandardItemModel(0,6,this);

	model->setHeaderData( COLUNA_L0, Qt::Horizontal, QObject::tr("L0") );
	model->setHeaderData( COLUNA_RAZAO_DE_ASPECTO, Qt::Horizontal, QObject::tr("Razão de Aspecto") );
	model->setHeaderData( COLUNA_RAZAO_DE_TRUNCAMENTO, Qt::Horizontal, QObject::tr("Razão de truncamento") );
	model->setHeaderData( COLUNA_PORCENTAGEM, Qt::Horizontal, QObject::tr("Porcentagem") );
	model->setHeaderData( COLUNA_QUANTIDADE, Qt::Horizontal, QObject::tr("Quantidade") );
	model->setHeaderData( COLUNA_COR, Qt::Horizontal, QObject::tr("Cor") );

	connect(model,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(manterProporcaoEntrePorcentagemEQuantidade(QStandardItem *)));
	connect(ui->textFracaoVazia,SIGNAL(textChanged (const QString &)),this,SLOT(manterProporcaoEntrePorcentagemEQuantidade()));

	ui->tableEspecificacao->setModel(model);


	TextBoxDelegate *texBox1 = new TextBoxDelegate(valPercentual);
	TextBoxDelegate *texBox2 = new TextBoxDelegate(valQuantidade);

	ui->tableEspecificacao->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableEspecificacao->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableEspecificacao->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	ui->tableEspecificacao->setItemDelegateForColumn(COLUNA_L0,texBox1);
	ui->tableEspecificacao->setItemDelegateForColumn(COLUNA_RAZAO_DE_ASPECTO,texBox1);
	ui->tableEspecificacao->setItemDelegateForColumn(COLUNA_RAZAO_DE_TRUNCAMENTO,texBox1);
	ui->tableEspecificacao->setItemDelegateForColumn(COLUNA_PORCENTAGEM,texBox1);
	ui->tableEspecificacao->setItemDelegateForColumn(COLUNA_QUANTIDADE,texBox2);

	ui->tableEspecificacao->setColumnWidth(COLUNA_L0,75);
	ui->tableEspecificacao->setColumnWidth(COLUNA_RAZAO_DE_ASPECTO,110);
	ui->tableEspecificacao->setColumnWidth(COLUNA_RAZAO_DE_TRUNCAMENTO,150);
	ui->tableEspecificacao->setColumnWidth(COLUNA_PORCENTAGEM,90);
	ui->tableEspecificacao->setColumnWidth(COLUNA_QUANTIDADE,75);
	ui->tableEspecificacao->setColumnWidth(COLUNA_COR,160);

}

double AdicionarPrismasSistematicamenteDialog::getPorcentagemFaseSolida(){
	bool valorValido  = false;
	double fracaoVazia = ui->textFracaoVazia->text().toDouble(&valorValido);
	if (valorValido)
		return 100-fracaoVazia;
	else return 0;
}

double AdicionarPrismasSistematicamenteDialog::getRazaoDeAspecto(int linha){
	QModelIndex coluna = this->model->index(linha,COLUNA_RAZAO_DE_ASPECTO);
	return this->model->data(coluna, Qt::DisplayRole).toDouble();

}
double AdicionarPrismasSistematicamenteDialog::getL0(int linha){
	QModelIndex coluna = this->model->index(linha,COLUNA_L0);
	return this->model->data(coluna, Qt::DisplayRole).toDouble();
}
double AdicionarPrismasSistematicamenteDialog::getRazaoDeTruncamento(int linha){
	QModelIndex coluna = this->model->index(linha,COLUNA_RAZAO_DE_TRUNCAMENTO);
	return this->model->data(coluna, Qt::DisplayRole).toDouble();
}

void AdicionarPrismasSistematicamenteDialog::manterProporcaoEntrePorcentagemEQuantidade(){
	QStandardItem *item;
	for(int i=0;i< this->model->rowCount();++i)
	{
		item = this->model->item(i,COLUNA_PORCENTAGEM);
		manterProporcaoEntrePorcentagemEQuantidade(item);
	}
}

void AdicionarPrismasSistematicamenteDialog::manterProporcaoEntrePorcentagemEQuantidade(QStandardItem *item){
	int row = item->row();
	QModelIndex colunaPorcentagem = this->model->index(row,COLUNA_PORCENTAGEM);
	QModelIndex colunaQtde = this->model->index(row,COLUNA_QUANTIDADE);

	double porcentagem = this->model->data(colunaPorcentagem, Qt::DisplayRole).toDouble();
	double porcentagemFaseSolida = getPorcentagemFaseSolida();

	double razaoDeAspecto = getRazaoDeAspecto(row);
	double L0 = getL0(row);
	double razaoDeTruncamento = getRazaoDeTruncamento(row);
	double volumeTotalDaCaixa = this->simulacao->getVolumeDaCaixa();
	double volumeDaFaseSolida = (volumeTotalDaCaixa * porcentagemFaseSolida)/100.0;
	double volumeDeUmPrismaTriangularTruncado = PrismaTriangularTruncado::calcularVolume(razaoDeAspecto,razaoDeTruncamento,L0);

	this->model->blockSignals(true);

	switch(item->column()){
		case COLUNA_QUANTIDADE:
			{
				int qtde = this->model->data(colunaQtde, Qt::DisplayRole).toInt();
				double volumeTotalDosPrismas =  qtde*volumeDeUmPrismaTriangularTruncado;
				double porcentagemDaFaseSolida = (100.0 * volumeTotalDosPrismas)/volumeDaFaseSolida;

				if (porcentagemDaFaseSolida>0)	this->model->setData(colunaPorcentagem,QVariant(porcentagemDaFaseSolida));
			}
			break;
		default:
			{
				double volumeTotalDosPrismas = (porcentagem*volumeDaFaseSolida)/100.0;				
				if (volumeDeUmPrismaTriangularTruncado>0){
					int qtde = volumeTotalDosPrismas/volumeDeUmPrismaTriangularTruncado;
					this->model->setData(colunaQtde,QVariant(qtde));
				}
			}
			break;
	}

	this->model->blockSignals(false);	
}
void AdicionarPrismasSistematicamenteDialog::adicionarDescricao(){

	int row = model->rowCount();
	model->insertRow(row);


	QModelIndex cell1 = model->index(row,COLUNA_L0);
	QModelIndex cell2 = model->index(row,COLUNA_RAZAO_DE_ASPECTO);
	QModelIndex cell3 = model->index(row,COLUNA_RAZAO_DE_TRUNCAMENTO);
	QModelIndex cell4 = model->index(row,COLUNA_PORCENTAGEM);
	QModelIndex cell5 = model->index(row,COLUNA_QUANTIDADE);
	QModelIndex cell6 = model->index(row,COLUNA_COR);

	model->setData(cell1,QVariant(0.0));
	model->setData(cell2,QVariant(0.0));
	model->setData(cell3,QVariant(0.0));
	model->setData(cell4,QVariant(0.0));
	model->setData(cell5,QVariant(0.0));
	model->setData(cell6,QColor("red"));


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

void AdicionarPrismasSistematicamenteDialog::adicionarPrismas(){

	double porcentagemFaseSolida = getPorcentagemFaseSolida();

	int linhas = this->model->rowCount();
	command = new AdicionarObjetosCommand(this->simulacao,porcentagemFaseSolida);

	for(int row=0;row<linhas;++row){
		QModelIndex colunaL0 = this->model->index(row,COLUNA_L0);
		QModelIndex colunaRazaoDeAspecto = this->model->index(row,COLUNA_RAZAO_DE_ASPECTO);
		QModelIndex colunaRazaoDeTruncamento = this->model->index(row,COLUNA_RAZAO_DE_TRUNCAMENTO);
		QModelIndex colunaPorcentagem = this->model->index(row,COLUNA_PORCENTAGEM);
		QModelIndex colunaCor = this->model->index(row,COLUNA_COR);


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