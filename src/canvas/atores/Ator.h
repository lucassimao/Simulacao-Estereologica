#ifndef ATOR_H
#define ATOR_H

#include <NxPhysics.h>

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

				virtual void draw(bool useShapeUserData) = 0;
				virtual bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition) = 0;

			};

		}
	}

}
#endif
