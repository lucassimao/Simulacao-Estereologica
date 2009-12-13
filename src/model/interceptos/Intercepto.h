#ifndef INTERCEPTO_H
#define INTERCEPTO_H

#include <NxPhysics.h>
#include <vector>
#include "..\..\defs.h"

using std::vector;

namespace simulacao{ 
	namespace model { 
		namespace interceptos{

			class Intercepto{
			protected:
				Cor cor;
			public:
				Intercepto();
				Intercepto(Cor);
			};

		}
	}
}

#endif