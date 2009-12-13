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

			class DrawVisitor{

			public:
				DrawVisitor();
				virtual void visit(Disco *disco)=0;
				virtual void visit(Poligono *poligono)=0;
			};
		}
	}
}
#endif