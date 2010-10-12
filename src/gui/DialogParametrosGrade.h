#ifndef DIALOGO_PARAMETROS_GRADE_H
#define DIALOGO_PARAMETROS_GRADE_H

#include <QDialog>
#include "..\..\forms\ui_configurarGradeDePontos.h"

namespace simulacao	{
	namespace gui {

		class DialogParametrosGrade: public QDialog{
			Q_OBJECT
		private:
			Ui_DlgParametrosDaGrade *ui;
		private slots:
			void cancelarAlteracoes();
			void salvarAlteracoes();
			void recalcularQuantidades();
			void recalcularDistancias();
		public:
			DialogParametrosGrade(QWidget *parent);
			~DialogParametrosGrade();


		};

	}

}

#endif