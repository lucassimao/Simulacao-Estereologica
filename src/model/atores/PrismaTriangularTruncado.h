#ifndef PRISMA_TRIANGULAR_TRUNCADO_H
#define PRISMA_TRIANGULAR_TRUNCADO_H

#include "../../utils/MeshFactory.h"
#include "../../utils/SegmentoDeReta.h"
#include "../../model/interceptos/Intercepto.h"
#include "Ator.h"

using namespace simulacao::utils;
using simulacao::model::interceptos::Intercepto;

namespace simulacao{

	namespace model{

		namespace atores {

			class PrismaTriangularTruncado: public Ator{
			public:
				PrismaTriangularTruncado(NxScene *cena,NxCCDSkeleton *ccds,MeshFactory *meshFactory);
				~PrismaTriangularTruncado();

				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);
				Intercepto* getIntercepto(NxVec3 planoGlobalPosition);
				inline NxVec3* getPosicaoGlobalDosVertices();
				double getVolume(){
					return (sqrt(3.0)/4.0)*( razaoDeAspecto *(1 -3*pow(razaoDeTruncamento,2))* pow(L0,3));
				}
				static double calcularVolume(double razaoDeAspecto,double razaoDeTruncamento,double L0);
			private:
				double razaoDeAspecto; // alpha
				double razaoDeTruncamento; // beta
				double L0;
				inline vector<SegmentoDeReta> getSegmentosDeRetaInterceptados(NxVec3);

			};
		}
	}
}

#endif