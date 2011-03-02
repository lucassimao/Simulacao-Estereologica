#include "distribuicaodeinterceptosdialog.h"

using namespace simulacao::gui;

DistribuicaoDeInterceptosDialog::DistribuicaoDeInterceptosDialog(QWidget *parent,ProcessadorDeClassesDeIntercepto *processador){
	ui = new Ui_DistribuicaoDeInterceptosDialog();
	ui->setupUi(this);
	this->processador = processador;

	criarCabecalhosDaTabela();
}

void DistribuicaoDeInterceptosDialog::sair(){
	this->close();
}

void DistribuicaoDeInterceptosDialog::salvar(){
	this->close();
}

void DistribuicaoDeInterceptosDialog::gerarDistribuicao(){
	QStringList labelDaLinha;
	int qtdeDeSubclasses = ui->textQtdeSubClasses->text().toInt();

	double menorInterceptoDeArea = this->processador->getMenorInterceptoDeArea();
	double maiorInterceptoDeArea = this->processador->getMaiorInterceptoDeArea();

	double intervaloDeClasse = (maiorInterceptoDeArea - menorInterceptoDeArea)/qtdeDeSubclasses;
	
	this->tableModel->removeRows(0, this->tableModel->rowCount());
	
	for(int i=0; i < qtdeDeSubclasses; ++i){
		int row = this->tableModel->rowCount();
		this->tableModel->insertRow(row);

		double subClasseMinima = menorInterceptoDeArea + i*intervaloDeClasse;
		double subClasseMaxima = subClasseMinima + intervaloDeClasse;
		labelDaLinha << tr("%1  |-  %2").arg(subClasseMinima).arg(subClasseMaxima);
		
		int qtdeDeClassesDeIntercepto = this->classesDeIntercepto.size();
		int qtdeTotalDeInterceptosDeAreaNoIntervalo = 0;

		for(int coluna=0; coluna < qtdeDeClassesDeIntercepto;++coluna){
			QModelIndex cell = this->tableModel->index(row,coluna);
			ClasseDeIntercepto classe = this->classesDeIntercepto[coluna];

			int qtde = processador->getQuantidadeDeInterceptosDeAreaNoIntervalo(subClasseMinima,subClasseMaxima,classe);
			qtdeTotalDeInterceptosDeAreaNoIntervalo += qtde;
			this->tableModel->setData(cell,QVariant(qtde));
		}

		int indiceDaColunaDeTotais = classesDeIntercepto.size();
		QModelIndex cellTotais = this->tableModel->index(row,indiceDaColunaDeTotais);
		this->tableModel->setData(cellTotais,QVariant(qtdeTotalDeInterceptosDeAreaNoIntervalo));
	}

	labelDaLinha << "Totais";
	int linhaDeTotalizacao = this->tableModel->rowCount();
	this->tableModel->insertRow(linhaDeTotalizacao);

	for(int coluna=0;coluna < this->tableModel->columnCount();++coluna){
		int qtdeDeInterceptos = 0;

		for(int linhaDaColuna= 0; linhaDaColuna < linhaDeTotalizacao;++linhaDaColuna){
			QModelIndex cell = this->tableModel->index(linhaDaColuna,coluna);
			qtdeDeInterceptos += this->tableModel->data(cell, Qt::DisplayRole).toInt();
		}
		
		QModelIndex cell = this->tableModel->index(linhaDeTotalizacao,coluna);
		this->tableModel->setData(cell,QVariant(qtdeDeInterceptos));
	}

	tableModel->setVerticalHeaderLabels(labelDaLinha);
}

void DistribuicaoDeInterceptosDialog::criarCabecalhosDaTabela(){
	this->classesDeIntercepto = processador->getClassesDeIntercepto();
	
	this->tableModel = new QStandardItemModel(0,classesDeIntercepto.size() + 1,this);
	ui->tableDistribuicao->setModel(tableModel);

	for(int idx=0; idx < classesDeIntercepto.size(); ++idx){ 
		ClasseDeIntercepto classe = classesDeIntercepto[idx];
		tableModel->setHeaderData( idx, Qt::Horizontal, 
			QObject::tr("D.eq(%1,%2,%3) = %4").arg(classe.razaoDeAspecto)
											.arg(classe.razaoDeTruncamento)
											.arg(classe.L0)
											.arg(classe.getDiametroEquivalente()) );
		
		ui->tableDistribuicao->setColumnWidth(idx,180);
	}

	int indiceDaColunaDeTotais = classesDeIntercepto.size();
	tableModel->setHeaderData(indiceDaColunaDeTotais,Qt::Horizontal,"Totais");
}

