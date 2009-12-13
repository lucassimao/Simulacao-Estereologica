#ifndef ESFERA_H
#define ESFERA_H

#include "..\..\model\interceptos\Intercepto.h"
#include "Ator.h"

using simulacao::model::interceptos::Intercepto;

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
				Intercepto* getIntercepto(NxVec3 planoGlobalPosition);

			};

		}
	}
}

#endif