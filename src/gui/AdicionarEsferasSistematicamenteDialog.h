#ifndef ADICIONAR_ESFERAS_SISTEMATICAMENTE_DIALOG_H
#define ADICIONAR_ESFERAS_SISTEMATICAMENTE_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "forms\ui_adicaoSistematizadaDeEsferas.h"
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
		private slots:
			void adicionarDescricao();
			void removerDescricao();
			void sair();
			void adicionarEsferas();
		public:
			AdicionarEsferasSistematicamenteDialog(QWidget *parent,SimulacaoCaixa *simulacao);
			AdicionarObjetosCommand * getCommand(){ return this->command;}



		};

	}

}

#endif