#ifndef SimulacaoCaixa_H
#define SimulacaoCaixa_H

#include <NxPhysics.h>
#include "../defs.h"
#include "../utils/MeshFactory.h"
#include "Simulacao.h"
#include "grade/Grade.h"
/**
* Definição de uma simulação simples, 
* objetos idênticos
*
*/
using namespace simulacao::utils;
using namespace simulacao::model::grade;

namespace simulacao{
	namespace model{

		class SimulacaoCaixa : public Simulacao
		{
		private:
			bool exibirCaixa;
			NxActor *caixa;
			MeshFactory *meshFactory;
			NxActor *atorPlanoDeCorte;
			NxPlane *shapePlanoDeCorte;
			NxActor *criarCaixa();
			
			bool exibirRetasTeste;
			bool exibirPontosTeste;
			void criarCCDS();

		public:
			NxActor* getCaixa(){
				return this->caixa;
			}
			NxActor* getPlanoDeCorte(){
				return this->atorPlanoDeCorte;
			}


			void exibirPlanoDeCorte();
			void esconderPlanoDeCorte();

			Grade * getGrade(){
				float alturaGrade = this->getPlanoDeCorte()->getGlobalPosition().y;
				return new Grade(9,-18,alturaGrade,10,50);
			
			}

			SimulacaoCaixa(void);
			~SimulacaoCaixa(void);

			void adicionarObjeto(TipoDeGrao,NxI64);
			void novoPlanoDeCorte();
			void selecionarGraosInterceptados();
			void removerGraos();

			bool getExibirRetasTeste(){ return this->exibirRetasTeste;}
			void setExibirRetasTeste(bool b){ this->exibirRetasTeste = b;}

			bool getExibirPontosTeste(){ return this->exibirPontosTeste;}
			void setExibirPontosTeste(bool b){ this->exibirPontosTeste = b;}


		};

	}

}

#endif