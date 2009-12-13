#ifndef SimulacaoCaixa_H
#define SimulacaoCaixa_H

#include <NxPhysics.h>
#include "../defs.h"
#include "../utils/MeshFactory.h"
#include "Simulacao.h"

/**
* Definição de uma simulação simples, 
* objetos idênticos
*
*/
using namespace simulacao::utils;

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
			
			//valores padrões
			double esferaRaio;
			double arestaCubo;
			double alturaPrisma;
			double basePrisma;

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

			SimulacaoCaixa(void);
			~SimulacaoCaixa(void);

			void adicionarObjeto(TipoDeGrao,NxI64);
			void novoPlanoDeCorte();
			void selecionarGraosInterceptados();
			void selecionarInterceptacoes();
			void removerGraos();

			double getEsferaRaio(){ return this->esferaRaio;}
			void setEsferaRaio(double novoRaio){ this->esferaRaio = novoRaio;}

			double getArestaCubo(){ return this->arestaCubo;}
			void setArestaCubo(double novaAresta){ this->arestaCubo = novaAresta;}
		};

	}

}

#endif