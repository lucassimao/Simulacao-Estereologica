#ifndef CILINDRO_H
#define CILINDRO_H

#include "Ator.h"

namespace simulacao{
	namespace model{
		namespace atores{

			class Capsula : public Ator
			{
			private:
				double raio_base;
				double altura;
			public:
				Capsula(NxScene *);
				~Capsula(void);
				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);


			};

		}
	}
}

#endif