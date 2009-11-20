#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "..\model\SimulacaoCaixa.h"
#include "..\canvas\CaixaGraosGLWidget.h"
#include "forms\ui_gui.h"

class QLabel;
using namespace simulacao::model;
using namespace simulacao::canvas;

namespace simulacao	{
	namespace gui {

		class MainWindow: public QMainWindow{
			Q_OBJECT
		private:
			//TIRAR ISSO!!!!!!!!!!!!!!!!!!!!!!!!!
			CaixaGraosGLWidget *view;
			QLabel *statusTipoSimulacao;
			QLabel *statusQtdeObjetos;
			Ui_MainWindow *ui;
			//TIRAR ISSO!!!!!!!!!!!!!!!!!!!!!!!!!
			SimulacaoCaixa *simulacao;

			void criarCanvas();
			void atualizarQuantidadeDeGraosEmCena();
		private slots:
			void adicionarObjetos();
			void pararSimulacao();
			void limparSimulacao();
			void novoPlanoDeCorte();
			void exibirSobre();
			void exibirGraosInterceptados();
			void configurarParametros();
			void exibirInterceptos();

			void exibirPlanoDeCorte(bool);
			void usarGravidade(bool);
			void mostrarCaixa(bool);
			void usarTamanhoDeGraoAleatorio(bool);
		public:
			MainWindow();
			~MainWindow();


		};

	}

}

#endif