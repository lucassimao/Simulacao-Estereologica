#ifndef ADICAO_SISTEMATICA_DE_PRISMAS_COM_VOLUME_DE_CAIXA_AJUSTAVEL_DIALOG_H
#define ADICAO_SISTEMATICA_DE_PRISMAS_COM_VOLUME_DE_CAIXA_AJUSTAVEL_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "..\..\forms\ui_adicaoSistematizadaDePrismasComAjusteDaCaixa.h"
#include "..\model\AdicionarObjetosCommand.h"
#include "..\model\SimulacaoCaixa.h"

using namespace simulacao::model;

namespace simulacao	{
	namespace gui {

		class AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog: public QDialog{
			Q_OBJECT
		private:
			Ui_AdicionarPrismasSistematicamenteComAjusteDeCaixaDialog *ui;
			QStandardItemModel *model;
			AdicionarObjetosCommand *command;
			SimulacaoCaixa *simulacao;
		private slots:
			void adicionarDescricao();
			void removerDescricao();
			void sair();
			void adicionarPrismas();
			void atualizarLabelVolumeFaseSolida();
			void atualizarLabelLadoDaCaixa();
		private:
			double getPorcentagemFaseSolida();
			double getVolumeDaFaseSolida();
			double getRazaoDeAspecto(int linha);
			double getL0(int linha);
			double getRazaoDeTruncamento(int linha);
			int getQuantidade(int linha);
			QColor getCor(int linha);
			void configurarModeloDaTabela();
			void configurarCamadaDeViewDaTabela(QDoubleValidator *valPercentual,QIntValidator *valQuantidade);
			void configurarEditorDeCores();
			

			#define COLUNA_L0 0
			#define COLUNA_RAZAO_DE_ASPECTO 1
			#define COLUNA_RAZAO_DE_TRUNCAMENTO 2
			#define COLUNA_QUANTIDADE 3
			#define COLUNA_COR 4

		public:
			AdicaoSistematicaDePrismasComVolumeDeCaixaAjustavelDialog(QWidget *parent,SimulacaoCaixa *simulacao);
			AdicionarObjetosCommand * getCommand(){ return this->command;}



		};

	}

}

#endif