#ifndef DIALOGO_PARAMETROS_CUBO_H
#define DIALOGO_PARAMETROS_CUBO_H

#include <QDialog>
#include "..\model\SimulacaoCaixa.h"
#include "forms\ui_cuboParametros.h"

using namespace simulacao::model;


namespace simulacao	{
	namespace gui {

		class DialogParametrosCubo: public QDialog{
			Q_OBJECT
		private:
			Ui_WinParametrosCubo *ui;
			SimulacaoCaixa *simulacao;

		private slots:
			void salvarAresta();
		public:
			DialogParametrosCubo(QWidget *parent,SimulacaoCaixa *simulacao);
			~DialogParametrosCubo();


		};

	}

}

#endif