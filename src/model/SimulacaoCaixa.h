#ifndef SimulacaoCaixa_H
#define SimulacaoCaixa_H

#include <NxPhysics.h>
#include "../defs.h"
#include "../utils/MeshFactory.h"
#include "Simulacao.h"

/**
* Defini��o de uma simula��o simples, 
* objetos id�nticos
*
*/
using namespace simulacao::utils;

namespace simulacao{
	namespace model{

		class SimulacaoCaixa : public Simulacao
		{
		private:
			bool exibirRetasTeste;
			bool exibirPontosTeste;
			bool exibirCaixa;
			bool exibirTampaCaixa;
			bool usarGraosAleatorios;
			NxActor *caixa;
			MeshFactory *meshFactory;
			NxActor *atorPlanoDeCorte;
			NxPlane *shapePlanoDeCorte;
			NxActor *criarCaixa();
			void criarCCDS();

		public:
			NxActor* getCaixa(){
				return this->caixa;
			}
			NxActor* getPlanoDeCorte(){
				return this->atorPlanoDeCorte;
			}
			void setUsarGraosAleatorios(){
				this->usarGraosAleatorios = true;
			}
			bool getUsarGraosAleatorios(){
				return this->usarGraosAleatorios;
			}

			void exibirPlanoDeCorte();
			void esconderPlanoDeCorte();

			SimulacaoCaixa(void);
			~SimulacaoCaixa(void);

			void adicionarObjeto(TipoDeGrao,NxI64);
			void novoPlanoDeCorte();
			void selecionarGraosInterceptados();
			void removerGraos();
		};

	}

}

#endif