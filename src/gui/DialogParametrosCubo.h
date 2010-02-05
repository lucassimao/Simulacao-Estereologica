#ifndef DIALOGO_PARAMETROS_CUBO_H
#define DIALOGO_PARAMETROS_CUBO_H

#include <QDialog>
#include "forms\ui_cuboParametros.h"

namespace simulacao	{
	namespace gui {

		class DialogParametrosCubo: public QDialog{
			Q_OBJECT
		private:
			Ui_WinParametrosCubo *ui;

		private slots:
			void salvarAresta();
		public:
			DialogParametrosCubo(QWidget *parent);
			~DialogParametrosCubo();


		};

	}

}

#endif