#ifndef MATHVISITOR_H
#define MATHVISITOR_H

namespace simulacao {
	namespace model {
		namespace interceptos{
			class Disco;
			class Poligono;
		}
	}
}
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