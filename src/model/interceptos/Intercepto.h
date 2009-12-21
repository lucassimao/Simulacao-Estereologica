#ifndef INTERCEPTO_H
#define INTERCEPTO_H

#include <NxPhysics.h>
#include <vector>
#include "..\..\defs.h"

using std::vector;

namespace simulacao{
	namespace canvas{
		namespace drawVisitor{
			class DrawVisitor;
		}}}

using namespace simulacao::canvas::drawVisitor;

namespace simulacao{ 
	namespace model { 
		namespace interceptos{

			class Intercepto{
			protected:
				Cor cor;
			public:
				Intercepto();
				Intercepto(Cor);
				virtual void accept(DrawVisitor *) = 0;
				Cor getCor() const { return cor;};
			};

		}
	}
}

#endif