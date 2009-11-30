#ifndef PRISMA_TRIANGULAR_H
#define PRISMA_TRIANGULAR_H

#include <vector>
#include "../../utils/MeshFactory.h"
#include "Ator.h"

using namespace simulacao::utils;
using std::vector;

namespace simulacao{

	namespace model{

		namespace atores {

			class PrismaTriangular: public Ator{
			public:
				PrismaTriangular(NxScene *cena,int h,int b,NxCCDSkeleton *ccds,MeshFactory *meshFactory);
				~PrismaTriangular();
				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);
				vector<NxVec3> getInterceptacoes(NxVec3 planoGlobalPosition);
			private:
				int altura;
				int base;

			};
		}
	}
}

#endif