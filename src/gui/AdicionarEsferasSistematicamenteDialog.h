#ifndef ADICIONAR_ESFERAS_SISTEMATICAMENTE_DIALOG_H
#define ADICIONAR_ESFERAS_SISTEMATICAMENTE_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "forms\ui_adicaoSistematizadaDeEsferas.h"

namespace simulacao	{
	namespace gui {

		class AdicionarEsferasSistematicamenteDialog: public QDialog{
			Q_OBJECT
		private:
			Ui_AdicionarEsferasSistematicamenteDialog *ui;
			QStandardItemModel *model;
		private slots:
			void adicionarDescricao();
			void removerDescricao();
			void sair();
			void adicionarEsferas();
		public:
			AdicionarEsferasSistematicamenteDialog(QWidget *parent);


		};

	}

}

#endif