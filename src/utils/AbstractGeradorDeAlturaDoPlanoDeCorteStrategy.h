#ifndef ABSTRACT_GERADOR_DE_ALTURA_DO_PLANO_DE_CORTE_H
#define ABSTRACT_GERADOR_DE_ALTURA_DO_PLANO_DE_CORTE_H


namespace simulacao{

	namespace utils{
	
		/**
		 * Representa uma estrat�gia abstrata para gera��o
		 * de alturas para os planos de corte
		 */
		class AbstractGeradorDeAlturaDoPlanoDeCorteStrategy{
		
		protected:
			double h0, h1; // altura m�nima e m�xima do plano de corte, respectivamente
		public:
			AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(double h0,double h1);
			virtual double novaAltura() = 0;
		};

	}

}


#endif