#ifndef PLANO_DE_CORTE_H
#define PLANO_DE_CORTE_H

#include "Ator.h"
#include "..\Parametros.h"
#include "..\..\utils\Observer.h"
#include "..\grade\Grade.h"

using namespace simulacao::model;
using namespace simulacao::model::grade;
using namespace simulacao::utils;

namespace simulacao{
	namespace model{
		namespace atores{

			class PlanoDeCorte : public Ator,public Observer
			{
			private:
				Grade *grade;
				double altura;

				Grade* novaGrade(float h);
			public:
				PlanoDeCorte(NxActor * nxActor);
				~PlanoDeCorte(void);
				
				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);
				Intercepto* getIntercepto(NxVec3 planoGlobalPosition) { return NULL;}
				NxActor *getNxActor(){ return this->ator;}
				void update(Observable* obs);
				
				Grade *getGrade();
				void setAltura(double);
				double getVolume(){
					return 0;
				}

			};

		}
	}
}

#endif