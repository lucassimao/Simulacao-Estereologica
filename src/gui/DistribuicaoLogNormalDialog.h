#ifndef DISTRIBUICAO_LOG_NORMAL_DIALOG_H
#define DISTRIBUICAO_LOG_NORMAL_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "..\..\forms\ui_distribuicaoLogNormal.h"
#include "..\model\AdicionarObjetosCommand.h"
#include "..\model\SimulacaoCaixa.h"

using namespace simulacao::model;

namespace simulacao	{
	namespace gui {

		class DistribuicaoLogNormalDialog: public QDialog{
			Q_OBJECT
		private:
			Ui_DistribuicaoLogNormalDialog *ui;
			QStandardItemModel *model;
			AdicionarObjetosCommand *command;
			SimulacaoCaixa *simulacao;

			void configurarEditorDeCores();
			void configurarModeloDaTabela();
			void configurarCamadaDeViewDaTabela();
			void configurarValidadoresDosCamposDeTexto();

			float getX0();
			float getXn();
			double getMi();
			double getSigma();
			double getN0();
			float getDeltaX();
			double getRazaoDeAspectoInicial();
			double getRazaoDeTruncamentoInicial();
			double calcularDistribuicaoLogNormal(double x, double mi, double sigma);
			
			void configurarOrdemDoTab();
			private slots:
				void criarDistribuicaoDeGraos();
				void sair();	
				void adicionarPrismas();
				void calcularL0s();

			#define COLUNA_X 0
			#define COLUNA_RAZAO_DE_ASPECTO 1
			#define COLUNA_RAZAO_DE_TRUNCAMENTO 2
			#define COLUNA_L0 3
			#define COLUNA_IMAGEM_NORMALIZADA 4
			#define COLUNA_QUANTIDADE 5
			#define COLUNA_COR 6
		public:
			DistribuicaoLogNormalDialog(QWidget *parent,SimulacaoCaixa *simulacao);
			AdicionarObjetosCommand * getCommand(){ return this->command;}

		};

	}

}

#endif