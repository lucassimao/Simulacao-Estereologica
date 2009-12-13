#ifndef CUBO_H
#define CUBO_H

#include "Ator.h"
#include "..\..\model\interceptos\Intercepto.h"

using simulacao::model::interceptos::Intercepto;

namespace simulacao{
	namespace model{
		namespace atores{

			class Cubo : public Ator
			{
			private:
				double lado;
			public:
				Cubo(NxScene *,double aresta);
				~Cubo(void);
				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);
				Intercepto* getIntercepto(NxVec3 planoGlobalPosition);

			};

		}
	}
}

#endif