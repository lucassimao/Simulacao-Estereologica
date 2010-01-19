#ifndef INTERCEPTO_H
#define INTERCEPTO_H

#include <NxPhysics.h>
#include <vector>
#include "..\..\defs.h"

using std::vector;

namespace simulacao{
	namespace canvas{
		namespace drawVisitor{
			class AbstractDrawVisitor;
		}}}

using namespace simulacao::canvas::drawVisitor;

namespace simulacao{
	namespace math{
		namespace mathVisitor{
			class AbstractMathVisitor;
		}}}

using namespace simulacao::math::mathVisitor;

namespace simulacao{ 
	namespace model { 
		namespace interceptos{

			class Intercepto{
			protected:
				Cor cor;
			public:
				Intercepto();
				Intercepto(Cor);

				virtual void accept(AbstractDrawVisitor *) = 0;
				virtual void accept(AbstractMathVisitor *) = 0;
				virtual double getArea() = 0;
				virtual double getPerimetro() = 0;

				Cor getCor() const { return cor;};
			};

		}
	}
}

#endif