#ifndef CONE_H
#define CONE_H

#include "Ator.h"

namespace simulacao{
	namespace model{
		namespace atores{

			class Cone : public Ator
			{
			public:
				Cone(NxScene *);
				~Cone(void);
				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);


			};

		}
	}
}

#endif