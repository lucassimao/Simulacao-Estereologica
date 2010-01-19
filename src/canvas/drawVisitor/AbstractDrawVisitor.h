#ifndef DRAWVISITOR_H
#define DRAWVISITOR_H

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
	namespace canvas {
		namespace drawVisitor {

			class AbstractDrawVisitor{

			public:
				AbstractDrawVisitor();
				virtual inline void visit(Disco *disco)=0;
				virtual inline void visit(Poligono *poligono)=0;
			};
		}
	}
}
#endif