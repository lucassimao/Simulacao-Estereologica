#ifndef SimulacaoSimples_H
#define SimulacaoSimples_H

#include <NxPhysics.h>
#include "../defs.h"
#include "Simulacao.h"

/**
* Definição de uma simulação simples, 
* objetos idênticos
*
*/
namespace simulacao{
	namespace model{

		class SimulacaoSimples : public Simulacao
		{
		private:
			NxActor *plano;
			NxActor *caixa;

			NxActor * criarPlano();
			NxActor *criarCaixa();
		public:
			SimulacaoSimples(void);
			~SimulacaoSimples(void);

		};

	}

}

#endif