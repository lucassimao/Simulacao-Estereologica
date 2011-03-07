#ifndef DISTRIBUICAO_DE_INTERCEPTOS_DIALOG_H
#define DISTRIBUICAO_DE_INTERCEPTOS_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "..\..\forms\ui_distribuicaoDeInterceptos.h"
#include "..\exportador\ProcessadorDeClassesDeIntercepto.h"

using namespace simulacao::exportador;

namespace simulacao	{
	namespace gui {

		class DistribuicaoDeInterceptosDialog: public QDialog{
			Q_OBJECT
		private:
			Ui_DistribuicaoDeInterceptosDialog *ui;
			ProcessadorDeClassesDeIntercepto *processador;

		private slots:
			void sair();
			void salvar();
			void gerarDistribuicao();
		private:
			QStandardItemModel *tableModel;
			vector<ClasseDeGrao> classesDeGrao;

			void criarCabecalhosDaTabela();
			void limparTabela();

		public:
			DistribuicaoDeInterceptosDialog(QWidget *parent,ProcessadorDeClassesDeIntercepto *processador);

		};
	}

}

#endif