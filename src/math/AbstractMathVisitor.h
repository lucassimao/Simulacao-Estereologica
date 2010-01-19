#ifndef MATHVISITOR_H
#define MATHVISITOR_H

#include "..\model\interceptos\Poligono.h"
#include "..\model\interceptos\Disco.h"

using namespace simulacao::model::interceptos;

namespace simulacao{
	namespace math {
		namespace mathVisitor {

			class AbstractMathVisitor{
			public:
				AbstractMathVisitor();
				virtual inline void visit(Disco *disco)=0;
				virtual inline void visit(Poligono *poligono)=0;
			};
		}
	}
}
#endif