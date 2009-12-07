#ifndef PRISMA_TRIANGULAR_H
#define PRISMA_TRIANGULAR_H

#include <vector>
#include "../../utils/MeshFactory.h"
#include "../../canvas/interceptos/Intercepto.h"
#include "Ator.h"

using namespace simulacao::utils;
using std::vector;
using simulacao::canvas::interceptos::Intercepto;

namespace simulacao{

	namespace model{

		namespace atores {

			class PrismaTriangular: public Ator{
			public:
				PrismaTriangular(NxScene *cena,int h,int b,NxCCDSkeleton *ccds,MeshFactory *meshFactory);
				~PrismaTriangular();
				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);
				Intercepto* getIntercepto(NxVec3 planoGlobalPosition);
			private:
				int altura;
				int base;

			};
		}
	}
}

#endif