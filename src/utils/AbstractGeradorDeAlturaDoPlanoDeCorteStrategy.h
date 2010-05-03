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
			double h0, h1; // altura mínima e máxima do plano de corte, respectivamente
		public:
			AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(double h0,double h1);
			virtual double novaAltura() = 0;
		};

	}

}


#endif