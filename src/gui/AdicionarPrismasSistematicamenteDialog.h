#ifndef ADICIONAR_PRISMAS_SISTEMATICAMENTE_DIALOG_H
#define ADICIONAR_PRISMAS_SISTEMATICAMENTE_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "forms\ui_adicaoSistematizadaDePrismas.h"
#include "..\model\AdicionarObjetosCommand.h"
#include "..\model\SimulacaoCaixa.h"

using namespace simulacao::model;

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
		public:
			AdicionarPrismasSistematicamenteDialog(QWidget *parent,SimulacaoCaixa *simulacao);
			AdicionarObjetosCommand * getCommand(){ return this->command;}



		};

	}

}

#endif