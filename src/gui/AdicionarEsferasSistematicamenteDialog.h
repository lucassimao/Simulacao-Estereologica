#ifndef ADICIONAR_ESFERAS_SISTEMATICAMENTE_DIALOG_H
#define ADICIONAR_ESFERAS_SISTEMATICAMENTE_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "..\..\forms\ui_adicaoSistematizadaDeEsferas.h"
#include "..\model\AdicionarObjetosCommand.h"
#include "..\model\SimulacaoCaixa.h"

using namespace simulacao::model;

namespace simulacao	{
	namespace gui {

		class AdicionarEsferasSistematicamenteDialog: public QDialog{
			Q_OBJECT
		private:
			Ui_AdicionarEsferasSistematicamenteDialog *ui;
			QStandardItemModel *model;
			AdicionarObjetosCommand *command;
			SimulacaoCaixa *simulacao;

			double getPorcentagemFaseSolida();
			double getRaio(int row);

			#define COLUNA_RAIO 0
			#define COLUNA_PORCENTAGEM 1
			#define COLUNA_QUANTIDADE 2
			#define COLUNA_COR 3

		private slots:
			void adicionarDescricao();
			void removerDescricao();
			void sair();
			void adicionarEsferas();
			void manterProporcaoEntrePorcentagemEQuantidade(QStandardItem *);
			void manterProporcaoEntrePorcentagemEQuantidade();
		public:
			AdicionarEsferasSistematicamenteDialog(QWidget *parent,SimulacaoCaixa *simulacao);
			AdicionarObjetosCommand * getCommand(){ return this->command;}



		};

	}

}

#endif