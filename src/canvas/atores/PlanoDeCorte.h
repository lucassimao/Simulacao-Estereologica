#ifndef PLANO_DE_CORTE_H
#define PLANO_DE_CORTE_H

#include "Ator.h"

namespace simulacao{
	namespace model{
		namespace atores{

			class PlanoDeCorte : public Ator
			{
			public:
				PlanoDeCorte(NxScene *);
				~PlanoDeCorte(void);
				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);


			};

		}
	}
}

#endif