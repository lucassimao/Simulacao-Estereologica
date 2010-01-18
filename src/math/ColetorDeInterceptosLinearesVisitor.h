#ifndef COLETOR_DE_INTERCEPTOS_LINEARES_VISITOR_H
#define COLETOR_DE_INTERCEPTOS_LINEARES_VISITOR_H

#include <vector>
#include "AbstractMathVisitor.h"
#include "..\model\grade\Grade.h"

using std::vector;
using namespace simulacao::model::grade;

namespace simulacao{
	namespace math{
		namespace mathVisitor{

			class ColetorDeInterceptosLinearesVisitor : public AbstractMathVisitor{
			private:
				vector<double> interceptosLineares;
				Grade *grade;
			public:
				ColetorDeInterceptosLinearesVisitor(Grade *);

				inline void visit(Disco *disco);
				inline void visit(Poligono *poligono);

				vector<double> getInterceptosLineares() const { return this->interceptosLineares;};
			};

		}
	}
}

#endif