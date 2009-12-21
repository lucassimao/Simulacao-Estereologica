#ifndef CUBO_H
#define CUBO_H

#include <set>
#include "Ator.h"
#include "..\..\utils\Vetor.h"
#include "..\..\utils\SegmentoDeReta.h"
#include "..\..\model\interceptos\Intercepto.h"

using simulacao::model::interceptos::Intercepto;
using std::set;

namespace simulacao{
	namespace model{
		namespace atores{

			class Cubo : public Ator
			{
			private:
				double lado;
				inline NxVec3* getPosicaoGlobalDosVertices();
				inline vector<SegmentoDeReta> getSegmentosDeRetaInterceptados(NxVec3);
			public:
				Cubo(NxScene *,double aresta);
				~Cubo(void);
				bool estaInterceptadoPeloPlano(NxVec3);
				Intercepto* getIntercepto(NxVec3);

			};

		}
	}
}

#endif