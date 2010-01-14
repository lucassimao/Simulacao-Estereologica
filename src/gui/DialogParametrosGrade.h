#ifndef DIALOGO_PARAMETROS_GRADE_H
#define DIALOGO_PARAMETROS_GRADE_H

#include <QDialog>
#include "..\model\SimulacaoCaixa.h"
#include "forms\ui_configurarGradeDePontos.h"

using namespace simulacao::model;


namespace simulacao	{
	namespace gui {

		class DialogParametrosGrade: public QDialog{
			Q_OBJECT
		private:
			Ui_DlgParametrosGradeDePontosELinhaTeste *ui;
			SimulacaoCaixa *simulacao;

		private slots:
			void cancelarAlteracoes();
			void salvarAlteracoes();
		public:
			DialogParametrosGrade(QWidget *parent,SimulacaoCaixa *simulacao);
			~DialogParametrosGrade();


		};

	}

}

#endif