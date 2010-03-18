#ifndef GERADOR_DE_ALTURA_ALEATORIA_DO_PLANO_DE_CORTE_H
#define GERADOR_DE_ALTURA_ALEATORIA_DO_PLANO_DE_CORTE_H

#include "AbstractGeradorDeAlturaDoPlanoDeCorteStrategy.h"

namespace simulacao{

	namespace utils{
		/**
		 *
		 * Esta classe representa uma estrat�gia onde
		 * s�o geradas alturas aleat�rias para o plano de corte. 
		 *
         */	
		class GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy: public AbstractGeradorDeAlturaDoPlanoDeCorteStrategy{

		public:
			GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy(float h0,float h1);
			float novaAltura() ;
		};

	}

}


#endif