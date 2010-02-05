#ifndef PRISMA_TRIANGULAR_H
#define PRISMA_TRIANGULAR_H

#include "../../utils/MeshFactory.h"
#include "../../utils/SegmentoDeReta.h"
#include "../../model/interceptos/Intercepto.h"
#include "Ator.h"

using namespace simulacao::utils;
using simulacao::model::interceptos::Intercepto;

namespace simulacao{

	namespace model{

		namespace atores {

			class PrismaTriangular: public Ator{
			public:
				PrismaTriangular(NxScene *cena,NxCCDSkeleton *ccds,MeshFactory *meshFactory);
				~PrismaTriangular();

				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);
				Intercepto* getIntercepto(NxVec3 planoGlobalPosition);
				inline NxVec3* getPosicaoGlobalDosVertices();
			private:
				int altura;
				int base;
				inline vector<SegmentoDeReta> getSegmentosDeRetaInterceptados(NxVec3);

			};
		}
	}
}

#endif