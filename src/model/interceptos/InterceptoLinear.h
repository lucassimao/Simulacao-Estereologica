#ifndef INTERCEPTO_LINEAR_H
#define INTERCEPTO_LINEAR_H

#include <NxPhysics.h>
#include <vector>
#include "..\..\defs.h"

using std::vector;

namespace simulacao{ 
	namespace model { 
		namespace interceptos{

			class InterceptoLinear{
			public:
				InterceptoLinear(Ponto p0,Ponto p1);
				const Ponto p0, p1;

				double tamanho();
			};


		}
	}
}

#endif