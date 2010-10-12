#ifndef ADICIONAR_PRISMAS_SISTEMATICAMENTE_DIALOG_H
#define ADICIONAR_PRISMAS_SISTEMATICAMENTE_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "..\..\forms\ui_adicaoSistematizadaDePrismas.h"
#include "..\model\AdicionarObjetosCommand.h"
#include "..\model\SimulacaoCaixa.h"

using namespace simulacao::model;

#define COLUNA_L0 0
#define COLUNA_RAZAO_DE_ASPECTO 1
#define COLUNA_RAZAO_DE_TRUNCAMENTO 2
#define COLUNA_PORCENTAGEM 3
#define COLUNA_QUANTIDADE 4
#define COLUNA_COR 5

namespace simulacao	{
	namespace gui {

		class AdicionarPrismasSistematicamenteDialog: public QDialog{
			Q_OBJECT
		private:
			Ui_AdicionarPrismasSistematicamenteDialog *ui;
			QStandardItemModel *model;
			AdicionarObjetosCommand *command;
			SimulacaoCaixa *simulacao;
		private slots:
			void adicionarDescricao();
			void removerDescricao();
			void sair();
			void adicionarEsferas();
			void manterProporcaoEntrePorcentagemEQuantidade(QStandardItem *);
			void manterProporcaoEntrePorcentagemEQuantidade();
		private:
			double getPorcentagemFaseSolida();
			double getRazaoDeAspecto(int linha);
			double getL0(int linha);
			double getRazaoDeTruncamento(int linha);
		public:
			AdicionarPrismasSistematicamenteDialog(QWidget *parent,SimulacaoCaixa *simulacao);
			AdicionarObjetosCommand * getCommand(){ return this->command;}



		};

	}

}

#endif