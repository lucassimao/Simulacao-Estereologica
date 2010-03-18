#ifndef GERADOR_SISTEMATICO_DE_ALTURA_DO_PLANO_DE_CORTE_H
#define GERADOR_SISTEMATICO_DE_ALTURA_DO_PLANO_DE_CORTE_H

#include "AbstractGeradorDeAlturaDoPlanoDeCorteStrategy.h"

namespace simulacao{

	namespace utils{
	
		/**
		 *
		 * Esta classe representa uma estratégia onde são geradas alturas sistemáticas
		 * para o plano de corte. 
		 *
         */
		class GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy: public AbstractGeradorDeAlturaDoPlanoDeCorteStrategy{
		
		private:
			// representa a variação da altura do plano de corte == (h1 - h0)/qtdePlanos
			float deltaH;
			float alturaCorrente;
		public:
			GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy(float h0,float h1, int qtdePlanos);
			float novaAltura();
		};

	};

}


#endif