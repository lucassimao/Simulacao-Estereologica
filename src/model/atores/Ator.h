#ifndef ATOR_H
#define ATOR_H

#include <NxPhysics.h>
#include <vector>
#include "..\..\model\interceptos\Intercepto.h"

using std::vector;
using simulacao::model::interceptos::Intercepto;

namespace simulacao{

	namespace model{

		namespace atores {

			class Ator
			{			
			protected:
				NxActor *ator;
			public:
				Ator();
				~Ator(void);

				virtual bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition) = 0;
				virtual Intercepto* getIntercepto(NxVec3 planoGlobalPosition) = 0;

			};

		}
	}

}
#endif
