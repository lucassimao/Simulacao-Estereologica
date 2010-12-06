#ifndef ADICAO_SISTEMATICA_DE_ESFERAS_COM_VOLUME_DE_CAIXA_AJUSTAVEL_DIALOG_H
#define ADICAO_SISTEMATICA_DE_ESFERAS_COM_VOLUME_DE_CAIXA_AJUSTAVEL_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "..\..\forms\ui_adicaoSistematizadaDeEsferasComAjusteDaCaixa.h"
#include "..\model\AdicionarObjetosCommand.h"
#include "..\model\SimulacaoCaixa.h"

using namespace simulacao::model;

namespace simulacao	{
	namespace gui {

		class AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog: public QDialog{
			Q_OBJECT
		private:
			Ui_AdicionarEsferasSistematicamenteComAjusteDaCaixaDialog *ui;
			QStandardItemModel *model;
			AdicionarObjetosCommand *command;
			SimulacaoCaixa *simulacao;
		private slots:
			void adicionarDescricao();
			void removerDescricao();
			void sair();
			void adicionarEsferas();
			void atualizarLabelVolumeFaseSolida();
			void atualizarLabelLadoDaCaixa();
		private:
			double getPorcentagemFaseSolida();
			double getVolumeDaFaseSolida();
			double getRaio(int linha);
			int getQuantidade(int linha);
			QColor getCor(int linha);
			void configurarModeloDaTabela();
			void configurarCamadaDeViewDaTabela(QDoubleValidator *valPercentual,QIntValidator *valQuantidade);
			void configurarEditorDeCores();
			

			#define COLUNA_RAIO 0
			#define COLUNA_QUANTIDADE 1
			#define COLUNA_COR 2
		public:
			AdicaoSistematicaDeEsferasComVolumeDeCaixaAjustavelDialog(QWidget *parent,SimulacaoCaixa *simulacao);
			AdicionarObjetosCommand * getCommand(){ return this->command;}

		};

	}

}

#endif