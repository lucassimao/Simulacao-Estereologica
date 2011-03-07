#ifndef INTERCEPTO_DE_AREA_H
#define INTERCEPTO_DE_AREA_H

#include <NxPhysics.h>
#include <vector>
#include "..\..\defs.h"
#include "InterceptoLinear.h"
#include "..\grade\grade.h"

using std::vector;
using namespace simulacao::model::grade;

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

			class InterceptoDeArea{
			protected:
				Cor cor;
			public:
				InterceptoDeArea();
				InterceptoDeArea(Cor);

				virtual void accept(AbstractDrawVisitor *) = 0;
				virtual void accept(AbstractMathVisitor *) = 0;
				virtual InterceptoType getType() = 0;
				virtual double getArea() = 0;
				virtual double getPerimetro() = 0;

				Cor getCor() const { return cor;};
				virtual vector<InterceptoLinear*> getInterceptosLineares(Grade *grade) = 0;

			};

		}
	}
}

#endif