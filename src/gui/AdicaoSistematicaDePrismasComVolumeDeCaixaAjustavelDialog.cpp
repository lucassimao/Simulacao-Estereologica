#include <QDialog>
#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include <stdexcept>
#include "..\model\Parametros.h"
#include "ColorListEditor.h"
#include "TextBoxDelegate.h"
#include "AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog.h"
#include "..\model\SimulacaoCaixa.h"
#include "..\model\atores\PrismaTriangularTruncado.h"

using namespace simulacao::gui;
using namespace simulacao::model;
using namespace simulacao::model::atores;
using std::runtime_error;

AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog(QWidget *parent, SimulacaoCaixa *simulacao):QDialog(parent){
	ui = new Ui_AdicionarPrismasSistematicamenteComAjusteDeCaixaDialog();
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
	
	configurarEditorDeCores();
	configurarModeloDaTabela();
	configurarCamadaDeViewDaTabela(valPercentual,valQuantidade);

	connect(ui->textFracaoVazia,SIGNAL(textChanged (const QString &)),this,SLOT(atualizarLabelLadoDaCaixa()));

	setWindowTitle("Adição Sistematizada de Prismas com Volume da Caixa Ajustável");
	 
}

void AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::configurarEditorDeCores(){
    QItemEditorFactory *factory = new QItemEditorFactory;
    QItemEditorCreatorBase *colorListCreator = new QStandardItemEditorCreator<ColorListEditor>();
	factory->registerEditor(QVariant::Color, colorListCreator);
    QItemEditorFactory::setDefaultFactory(factory);
}
void AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::configurarCamadaDeViewDaTabela(QDoubleValidator *valPercentual,QIntValidator *valQuantidade){
	TextBoxDelegate *texBox1 = new TextBoxDelegate(valPercentual);
	TextBoxDelegate *texBox2 = new TextBoxDelegate(valQuantidade);

	ui->tableEspecificacao->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableEspecificacao->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableEspecificacao->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	ui->tableEspecificacao->setItemDelegateForColumn(COLUNA_L0,texBox1);
	ui->tableEspecificacao->setItemDelegateForColumn(COLUNA_RAZAO_DE_ASPECTO,texBox1);
	ui->tableEspecificacao->setItemDelegateForColumn(COLUNA_RAZAO_DE_TRUNCAMENTO,texBox1);
	ui->tableEspecificacao->setItemDelegateForColumn(COLUNA_QUANTIDADE,texBox2);

	ui->tableEspecificacao->setColumnWidth(COLUNA_L0,75);
	ui->tableEspecificacao->setColumnWidth(COLUNA_RAZAO_DE_ASPECTO,110);
	ui->tableEspecificacao->setColumnWidth(COLUNA_RAZAO_DE_TRUNCAMENTO,150);
	ui->tableEspecificacao->setColumnWidth(COLUNA_QUANTIDADE,75);
	ui->tableEspecificacao->setColumnWidth(COLUNA_COR,160);

}
void AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::configurarModeloDaTabela(){
	model = new QStandardItemModel(0,5,this);
 
	model->setHeaderData( COLUNA_L0, Qt::Horizontal, QObject::tr("L0") );
	model->setHeaderData( COLUNA_RAZAO_DE_ASPECTO, Qt::Horizontal, QObject::tr("Razão de Aspecto") );
	model->setHeaderData( COLUNA_RAZAO_DE_TRUNCAMENTO, Qt::Horizontal, QObject::tr("Razão de truncamento") );
	model->setHeaderData( COLUNA_QUANTIDADE, Qt::Horizontal, QObject::tr("Quantidade") );
	model->setHeaderData( COLUNA_COR, Qt::Horizontal, QObject::tr("Cor") );

	ui->tableEspecificacao->setModel(model);
	connect(model,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(atualizarLabelVolumeFaseSolida())); 
	connect(model,SIGNAL(itemChanged(QStandardItem *)),this,SLOT(atualizarLabelLadoDaCaixa())); 

}

double AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::getPorcentagemFaseSolida(){
	bool valorValido  = false;
	double fracaoVazia = ui->textFracaoVazia->text().toDouble(&valorValido);
	if (valorValido)
		return 100 - fracaoVazia;
	else{
		throw exception("Forneça uma fração de vazio válida");	
	}
}

double AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::getRazaoDeAspecto(int linha){
	QModelIndex coluna = this->model->index(linha,COLUNA_RAZAO_DE_ASPECTO);
	return this->model->data(coluna, Qt::DisplayRole).toDouble();

}
double AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::getL0(int linha){
	QModelIndex coluna = this->model->index(linha,COLUNA_L0);
	return this->model->data(coluna, Qt::DisplayRole).toDouble();
}
double AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::getRazaoDeTruncamento(int linha){
	QModelIndex coluna = this->model->index(linha,COLUNA_RAZAO_DE_TRUNCAMENTO);
	return this->model->data(coluna, Qt::DisplayRole).toDouble();
}
int AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::getQuantidade(int linha){
	QModelIndex coluna = this->model->index(linha, COLUNA_QUANTIDADE);
	return this->model->data(coluna, Qt::DisplayRole).toInt();
}

QColor AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::getCor(int linha){
	QModelIndex coluna = this->model->index(linha, COLUNA_COR);
	return qVariantValue<QColor>(this->model->data(coluna, Qt::DisplayRole));
}


void AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::adicionarDescricao(){

	int row = model->rowCount();
	model->insertRow(row);

	QModelIndex cell1 = model->index(row,COLUNA_L0);
	QModelIndex cell2 = model->index(row,COLUNA_RAZAO_DE_ASPECTO);
	QModelIndex cell3 = model->index(row,COLUNA_RAZAO_DE_TRUNCAMENTO);
	QModelIndex cell4 = model->index(row,COLUNA_QUANTIDADE);
	QModelIndex cell5 = model->index(row,COLUNA_COR);

	model->setData(cell1,QVariant(0.0));
	model->setData(cell2,QVariant(0.0));
	model->setData(cell3,QVariant(0.0));
	model->setData(cell4,QVariant(0.0));
	model->setData(cell5,QColor("red"));

}

void AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::removerDescricao(){
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

void AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::sair(){
	this->reject();
}

void AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::atualizarLabelVolumeFaseSolida(){
	this->ui->labelVolumeFaseSolida->setText(tr("%1").arg(getVolumeDaFaseSolida()));
}

void AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::atualizarLabelLadoDaCaixa(){
	try{
		double porcentagemFaseSolida = getPorcentagemFaseSolida()/100.0;
		double ladoDaCaixa =  pow(getVolumeDaFaseSolida()/porcentagemFaseSolida,1/3.0);
		this->ui->labelLadoDaCaixa->setText(tr("%1").arg(ladoDaCaixa));
	}catch(exception &e){
		QMessageBox *msg = new QMessageBox(QMessageBox::Information,tr("Atenção"),tr(e.what()),QMessageBox::Ok,this);
		msg->show();
	}
}

double AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::getVolumeDaFaseSolida(){
	int linhas = this->model->rowCount();
	double volumeTotal = 0;

	for(int row = 0; row < linhas; ++row){
		double L0 = getL0(row);
		double razaoDeAspecto = getRazaoDeAspecto(row);
		double razaoDeTruncamento = getRazaoDeTruncamento(row);
		int quantidade = getQuantidade(row);
		double volumeDoPrisma = PrismaTriangularTruncado::calcularVolume(razaoDeAspecto,razaoDeTruncamento,L0);
		volumeTotal += volumeDoPrisma*quantidade;		
	}
	return volumeTotal;

}


void AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog::adicionarPrismas(){
	try{
		double porcentagemFaseSolida = getPorcentagemFaseSolida()/100.0;

		int linhas = this->model->rowCount();
		this->command = new AdicionarObjetosCommand(this->simulacao,porcentagemFaseSolida);
		double volumeTotal = 0;

		for(int row = 0; row < linhas; ++row){
			double L0 = getL0(row);
			double razaoDeAspecto = getRazaoDeAspecto(row);
			double razaoDeTruncamento = getRazaoDeTruncamento(row);
			int quantidade = getQuantidade(row);
			QColor cor = getCor(row);
			double volumeDoPrisma = PrismaTriangularTruncado::calcularVolume(razaoDeAspecto,razaoDeTruncamento,L0);

			volumeTotal += volumeDoPrisma*quantidade;

			Cor c = {cor.red()/255.0f,cor.green()/255.0f,cor.blue()/255.0f};
			
			command->adicionarPrismas(L0,quantidade,c,razaoDeAspecto,razaoDeTruncamento);
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