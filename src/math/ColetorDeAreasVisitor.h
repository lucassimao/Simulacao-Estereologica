#ifndef COLETOR_DE_AREAS_VISITOR_H
#define COLETOR_DE_AREAS_VISITOR_H

#include <vector>
#include "AbstractMathVisitor.h"
#include "..\model\interceptos\Poligono.h"
#include "..\model\interceptos\Disco.h"
#include "..\model\grade\Grade.h"

using std::vector;
using namespace simulacao::model::grade;
using namespace simulacao::model::interceptos;

namespace simulacao{
	namespace math{
		namespace mathVisitor{

			class ColetorDeAreasVisitor : public AbstractMathVisitor{
			private:
				Grade *grade;
				vector<double> areas;
			public:
				ColetorDeAreasVisitor(Grade *);

				inline void visit(Disco *disco);
				inline void visit(Poligono *poligono);

				vector<double> getAreas() { return this->areas;};
			};

		}
	}
}

#endif