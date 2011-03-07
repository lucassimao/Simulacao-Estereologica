#ifndef PRISMA_TRIANGULAR_H
#define PRISMA_TRIANGULAR_H

#include "../../utils/MeshFactory.h"
#include "../../utils/SegmentoDeReta.h"
#include "../../model/interceptos/InterceptoDeArea.h"
#include "Ator.h"

using namespace simulacao::utils;
using simulacao::model::interceptos::InterceptoDeArea;

namespace simulacao{

	namespace model{

		namespace atores {

			class PrismaTriangular: public Ator{
			public:
				PrismaTriangular(NxScene *cena,MeshFactory *meshFactory,Cor cor);
				~PrismaTriangular();

				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);
				InterceptoDeArea* getIntercepto(NxVec3 planoGlobalPosition);
				inline NxVec3* getPosicaoGlobalDosVertices();
				double getVolume(){
					return this->altura*((pow(this->base,2.0)*sqrt(3.0))/4.0);
				}
			private:
				double altura;
				double base;
				inline vector<SegmentoDeReta> getSegmentosDeRetaInterceptados(NxVec3);
				

			};
		}
	}
}

#endif