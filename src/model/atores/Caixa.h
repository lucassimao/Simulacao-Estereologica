#ifndef CAIXA_H
#define CAIXA_H

#include "Ator.h"

namespace simulacao{
	namespace model{
		namespace atores{

			class Caixa : public Ator
			{
			public:
				Caixa(NxScene *);
				~Caixa(void);
				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);
				void draw(bool useShapeUserData);


			};

		}
	}
}

#endif