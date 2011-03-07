#ifndef PRISMA_TRIANGULAR_TRUNCADO_H
#define PRISMA_TRIANGULAR_TRUNCADO_H

#include "../../utils/MeshFactory.h"
#include "../../utils/SegmentoDeReta.h"
#include "../../model/interceptos/InterceptoDeArea.h"
#include "Ator.h"

using namespace simulacao::utils;
using simulacao::model::interceptos::InterceptoDeArea;

namespace simulacao{

	namespace model{

		namespace atores {

			class PrismaTriangularTruncado: public Ator{
			public:
				PrismaTriangularTruncado(NxScene *cena,MeshFactory *meshFactory,Cor cor);
				~PrismaTriangularTruncado();

				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);
				InterceptoDeArea* getIntercepto(NxVec3 planoGlobalPosition);
				inline NxVec3* getPosicaoGlobalDosVertices();
				double getVolume(){
					return PrismaTriangularTruncado::calcularVolume(razaoDeAspecto,razaoDeTruncamento,L0);
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