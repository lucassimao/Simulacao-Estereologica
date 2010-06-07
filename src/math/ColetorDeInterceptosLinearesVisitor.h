#ifndef COLETOR_DE_INTERCEPTOS_LINEARES_VISITOR_H
#define COLETOR_DE_INTERCEPTOS_LINEARES_VISITOR_H

#include <vector>
#include "..\model\interceptos\Poligono.h"
#include "..\model\interceptos\Disco.h"

#include "AbstractMathVisitor.h"
#include "..\model\grade\Grade.h"

using std::vector;
using namespace simulacao::model::grade;
using namespace simulacao::model::interceptos;

namespace simulacao{
	namespace math{
		namespace mathVisitor{

			class ColetorDeInterceptosLinearesVisitor : public AbstractMathVisitor{
			private:
				Grade *grade;
			public:
				vector<double> interceptosLineares;

				ColetorDeInterceptosLinearesVisitor(Grade *);

				inline void visit(Disco *disco);
				inline void visit(Poligono *poligono);

			};

		}
	}
}

#endif