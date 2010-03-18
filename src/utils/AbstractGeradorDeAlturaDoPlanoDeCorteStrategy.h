#ifndef ABSTRACT_GERADOR_DE_ALTURA_DO_PLANO_DE_CORTE_H
#define ABSTRACT_GERADOR_DE_ALTURA_DO_PLANO_DE_CORTE_H


namespace simulacao{

	namespace utils{
	
		/**
		 * Representa uma estratégia abstrata para geração
		 * de alturas para os planos de corte
		 */
		class AbstractGeradorDeAlturaDoPlanoDeCorteStrategy{
		
		protected:
			float h0, h1; // altura mínima e máxima do plano de corte, respectivamente
		public:
			AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(float h0,float h1);
			virtual float novaAltura() = 0;
		};

	}

}


#endif