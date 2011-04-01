#include <QFileDialog>
#include <QMessageBox>
#include "distribuicaodeinterceptosdialog.h"
#include "..\exportador\ExportadorParaArquivo.h"

using namespace simulacao::gui;
using namespace simulacao::exportador;

DistribuicaoDeInterceptosDialog::DistribuicaoDeInterceptosDialog(QWidget *parent,sqlite3 *db):QDialog(parent){
	ui = new Ui_DistribuicaoDeInterceptosDialog();
	ui->setupUi(this);
	this->setWindowFlags(Qt::Window);
	this->db = db;
	this->processador = new ProcessadorDeClassesDeIntercepto(db);
	if (this->processador->getTipoDeGraoNaSimulacao() == Esferico){
		ui->labelDiametro->setText("Tamanho do Grão(Diâmetro)");
	}
	criarCabecalhosDaTabela();
}



void DistribuicaoDeInterceptosDialog::sair(){
	this->close();
}

void DistribuicaoDeInterceptosDialog::salvar(){
	QString dir = QFileDialog::getExistingDirectory(this,"Selecione o diretório onde deseja armazenar a tabela");
	if (dir.trimmed().size()>0){
		int qtdeClassesDeIntercepto = this->ui->textQtdeClassesDeIntercepto->text().toInt();
		ExportadorParaArquivo exportador1(dir.toStdString(),this->db);
		exportador1.exportarTabelasDeProbabilidade(qtdeClassesDeIntercepto);
		QMessageBox::information(this, tr("Exportação concluída"),tr("As tabelas foram exportadas com sucesso!"));
	}
}

void DistribuicaoDeInterceptosDialog::limparTabela(){
	this->tableModel->removeRows(0, this->tableModel->rowCount());
}

void DistribuicaoDeInterceptosDialog::gerarDistribuicao(){
	TipoDeIntercepto tipoDeIntercepto;

	if (this->ui->radioButtonArea->isChecked()){
		tipoDeIntercepto = Area;
	}else if (this->ui->radioButtonComprimentoLinear->isChecked()){
		tipoDeIntercepto = Linear;
	}else if (this->ui->radioButtonPoro->isChecked()){
		tipoDeIntercepto = Poro;
	}else{
		tipoDeIntercepto = Perimetro;
	}

	double menorIntercepto = this->processador->getMenorIntercepto(tipoDeIntercepto);
	double maiorIntercepto = this->processador->getMaiorIntercepto(tipoDeIntercepto);
	int qtdeClassesDeIntercepto = this->ui->textQtdeClassesDeIntercepto->text().toInt();
	double intervaloDeClasse = (maiorIntercepto - menorIntercepto)/qtdeClassesDeIntercepto;
	
	limparTabela();

	QStringList labelDaLinha;
	for(int i=0; i < qtdeClassesDeIntercepto; ++i){
		int row = this->tableModel->rowCount();
		this->tableModel->insertRow(row);

		double limiteInferior = menorIntercepto + i*intervaloDeClasse;
		double limiteSuperior = limiteInferior + intervaloDeClasse;
		labelDaLinha << tr("%1  |-  %2").arg(limiteInferior).arg(limiteSuperior);
		
		int qtdeDeClassesDeGrao = this->classesDeGrao.size();
		int qtdeTotalDeInterceptosNoIntervalo = 0;

		for(int coluna=0; coluna < qtdeDeClassesDeGrao;++coluna){
			QModelIndex cell = this->tableModel->index(row,coluna);
			ClasseDeGrao *classe = this->classesDeGrao[coluna];

			int qtde = processador->getQuantidadeDeInterceptosNoIntervalo(limiteInferior,limiteSuperior,classe,tipoDeIntercepto);
			this->tableModel->setData(cell,QVariant(qtde));

			qtdeTotalDeInterceptosNoIntervalo += qtde; // acumulando p/ totalização
		}

		int indiceDaColunaDeTotais = this->classesDeGrao.size();
		QModelIndex cellTotais = this->tableModel->index(row,indiceDaColunaDeTotais);
		this->tableModel->setData(cellTotais,QVariant(qtdeTotalDeInterceptosNoIntervalo));
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
	this->classesDeGrao = processador->getClassesDeGrao();
	int qtdeColunas = this->classesDeGrao.size() + 1;

	this->tableModel = new QStandardItemModel(0,qtdeColunas,this);
	ui->tableDistribuicao->setModel(tableModel);

	for(int colunaIdx=0; colunaIdx < (qtdeColunas-1); ++colunaIdx){ 
		ClasseDeGrao *classe = this->classesDeGrao[colunaIdx];
		double diametroEquivalenteDaClasseDeGrao = classe->getDiametroEquivalente();

		tableModel->setHeaderData( colunaIdx , Qt::Horizontal, QObject::tr("%1").arg(diametroEquivalenteDaClasseDeGrao));
		ui->tableDistribuicao->setColumnWidth(colunaIdx,60);
	}

	int indiceDaColunaDeTotais = qtdeColunas-1;
	tableModel->setHeaderData(indiceDaColunaDeTotais,Qt::Horizontal,"Totais");
}

