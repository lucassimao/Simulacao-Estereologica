#ifndef ESFERA_H
#define ESFERA_H

#include "Ator.h"

namespace simulacao{
	namespace model{
		namespace atores{

			class Esfera : public Ator
			{
			private:
				double raio;
			public:
				Esfera(NxScene *,NxCCDSkeleton *ccds);
				~Esfera(void);
				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);

			};

		}
	}
}

#endif