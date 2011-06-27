#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include <QMainWindow>
#include "..\model\SimulacaoCaixa.h"
#include "..\canvas\glWidget\CaixaGraosGLWidget.h"
#include "..\..\forms\ui_gui.h"

class QLabel;
using namespace simulacao::model;
using namespace simulacao::canvas::glWidget;

namespace simulacao	{
	namespace gui {

		class MainWindow: public QMainWindow{
			Q_OBJECT
		private :
			//TIRAR ISSO !!!!!!!!!!!!!!!!!!!!!!!!!
			CaixaGraosGLWidget *view;
			QLabel *statusTipoSimulacao;
			QLabel *statusQtdeObjetos;
			Ui_MainWindow *ui;
			//TIRAR ISSO!!!!!!!!!!!!!!!!!!!!!!!!!
			SimulacaoCaixa *simulacao;

			inline void criarCanvas();
			inline void atualizarQuantidadeDeGraosEmCena();
			inline void resetarComponentes();
			
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
			void usarGraosAleatorios(bool);
			void novaSimulacao();
			void exibirRetasTeste(bool);
			void exibirPontosTeste(bool);
			void configurarGrade();
			void exibirVisaoSuperior(bool);
			void mudarEstrategiaDeAlturasDoPlanodeCorte(bool);
			void configurarCorDoPlanoDeCorte();

			void adicionarEsferasSistematicamente();
			void adicionarPrismasSistematicamente();
			void adicionarPrismasSistematicamenteComVolumeDaCaixaAjustavel();
			void adicionarEsferasSistematicamenteComVolumeDaCaixaAjustavel();
			void actionExecutarCortesSistematicos();
			void distribuicaoLogNormal();
			void exibirDistribuicaoDeInterceptos();

			void modificarInterpenetracao();
			void modificarAceleracaoDaGravidade();
		public:
			MainWindow();
			~MainWindow();


		};

	}

}

#endif