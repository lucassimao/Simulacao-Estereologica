#ifndef ESFERA_H
#define ESFERA_H

#include "..\..\model\interceptos\Intercepto.h"
#include "Ator.h"

using simulacao::model::interceptos::Intercepto;

namespace simulacao{
	namespace model{
		namespace atores{

			class Esfera : public Ator
			{
			private:
				double raio;
			public:
				Esfera(NxScene *,Cor cor);
				~Esfera(void);
				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);
				bool estaForaDaCaixa(NxVec3 globalPositionDaCaixa,double arestaDaCaixa);
				Intercepto* getIntercepto(NxVec3 planoGlobalPosition);
				double getVolume(){
					return Esfera::calcularVolume(this->raio);
				}
				static double calcularVolume(double raio){
					return (4.0/3.0)*3.14*pow(raio,3);
				}

			};

		}
	}
}

#endif