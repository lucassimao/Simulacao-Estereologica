#ifndef INTERCEPTO_H
#define INTERCEPTO_H

#include <NxPhysics.h>
#include <vector>
#include "designer\DrawCommand.h"
#include "..\..\defs.h"

using std::vector;
using namespace simulacao::canvas::interceptos::designer;

namespace simulacao{ 
	namespace canvas { 
		namespace interceptos{

			class Intercepto{
			protected:
				Cor cor;
			public:
				Intercepto();
				Intercepto(Cor);

				virtual DrawCommand* getDrawCommand() const = 0;
			};

		}
	}
}

#endif