#ifndef ATOR_H
#define ATOR_H

#include <NxPhysics.h>
#include <vector>
#include "..\..\defs.h"
#include "..\..\model\interceptos\InterceptoDeArea.h"

using std::vector;
using simulacao::model::interceptos::InterceptoDeArea;

namespace simulacao{

	namespace model{

		namespace atores {

			class Ator
			{			
			private:
				int atorID;
				static int nextActorID;

			protected:
				NxActor *ator;
				
			public:
				Ator();
				~Ator(void);
				
				Cor cor;
				bool estaForaDaCaixa(NxVec3 globalPositionDaCaixa,double arestaDaCaixa);
				virtual bool estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition) = 0;
				virtual InterceptoDeArea* getIntercepto(NxVec3 planoGlobalPosition) = 0;
				virtual double getVolume() = 0;
				int getAtorID(){ return this->atorID;};
			};

		}
	}

}
#endif
