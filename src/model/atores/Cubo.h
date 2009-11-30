#ifndef CUBO_H
#define CUBO_H

#include "Ator.h"

namespace simulacao{
	namespace model{
		namespace atores{

			class Cubo : public Ator
			{
			private:
				double lado;
			public:
				Cubo(NxScene *,NxCCDSkeleton *ccds);
				~Cubo(void);
				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);

			};

		}
	}
}

#endif