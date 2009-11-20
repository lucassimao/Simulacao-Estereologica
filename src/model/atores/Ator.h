#ifndef ATOR_H
#define ATOR_H

#include <NxPhysics.h>
#include <vector>

using std::vector;

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
				virtual vector<NxVec3> getInterceptacoes(NxVec3 planoGlobalPosition);

			};

		}
	}

}
#endif
