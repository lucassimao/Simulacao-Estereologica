#ifndef SimulacaoCaixa_H
#define SimulacaoCaixa_H

#include <NxPhysics.h>
#include "..\defs.h"
#include "..\utils\AbstractGeradorDeAlturaDoPlanoDeCorteStrategy.h"
#include "..\utils\MeshFactory.h"
#include "Simulacao.h"
#include "..\model\atores\PlanoDeCorte.h"
#include "grade\Grade.h"
#include "Parametros.h"
#include <QObject>
/**
* Definição de uma simulação simples, 
* objetos idênticos
*
*/
using namespace simulacao::utils;
using namespace simulacao::model::atores;
using namespace simulacao::model::grade;

namespace simulacao{
	namespace model{

		class SimulacaoCaixa : public QObject, public Simulacao
		{
		Q_OBJECT


		//private slots:
		//	void adicionarEsfera();

		private:
			NxActor *caixa;
			MeshFactory *meshFactory;
			PlanoDeCorte *atorPlanoDeCorte;
			AbstractGeradorDeAlturaDoPlanoDeCorteStrategy *alturaPlanoStrategy;
			bool exibirCaixa;
			bool _exibirPlanoDeCorte;
			bool exibirRetasTeste;
			bool exibirPontosTeste;
			//void criarCCDS();			

		public:
			void criarCaixa();
			NxActor* getCaixa(){
				return this->caixa;
			}
			PlanoDeCorte *getPlanoDeCorte(){
				if (this->_exibirPlanoDeCorte)
					return this->atorPlanoDeCorte;
				else
					return NULL;
			}


			void exibirPlanoDeCorte();
			void esconderPlanoDeCorte();

			Grade * getGrade(){
				return getPlanoDeCorte()->getGrade();			
			}
			
			double getVolumeFaseSolida();

			double getVolumeFaseLigante(){
				Parametros *params = Parametros::getInstance();
				return pow(params->getArestaDaCaixa(),3) - getVolumeFaseSolida();
			}
			
			double getVolumeDaCaixa(){
				Parametros *params = Parametros::getInstance();
				return pow(params->getArestaDaCaixa(),3);
			}

			SimulacaoCaixa(void);
			~SimulacaoCaixa(void);

			void adicionarObjeto(TipoDeGrao,NxI64,Cor);
			void novoPlanoDeCorte();
			void selecionarGraosInterceptados();
			void removerGraos();
			
			void setGeradorDeAlturaDoPlanoStrategy(AbstractGeradorDeAlturaDoPlanoDeCorteStrategy* strategy){
				this->alturaPlanoStrategy = strategy;
			}

			bool getExibirRetasTeste(){ return this->exibirRetasTeste;}
			void setExibirRetasTeste(bool b){ this->exibirRetasTeste = b;}

			bool getExibirPontosTeste(){ return this->exibirPontosTeste;}
			void setExibirPontosTeste(bool b){ this->exibirPontosTeste = b;}
		};

	}

}

#endif