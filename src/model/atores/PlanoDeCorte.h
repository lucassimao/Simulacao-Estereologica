#ifndef PLANO_DE_CORTE_H
#define PLANO_DE_CORTE_H

#include "Ator.h"
#include "..\grade\Grade.h"

using namespace simulacao::model::grade;
namespace simulacao{
	namespace model{
		namespace atores{

			class PlanoDeCorte : public Ator
			{
			private:
				Grade *grade;
			public:
				PlanoDeCorte(NxActor * nxActor);
				~PlanoDeCorte(void);
				
				bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition);
				Intercepto* getIntercepto(NxVec3 planoGlobalPosition) { return NULL;}
				NxActor *getNxActor(){ return this->ator;}
				
				Grade *getGrade(){
					if (!grade){
						float alturaGrade = this->ator->getGlobalPosition().y;
						this->grade = new Grade(9,-9,alturaGrade,10,50);
					}
					return grade;
				}

			};

		}
	}
}

#endif