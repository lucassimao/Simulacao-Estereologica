#ifndef DRAW_VISITOR_H
#define DRAW_VISITOR_H

// forward declaration, evitando referencia circular ...
namespace simulacao{
	namespace model{
		namespace interceptos{
			class Disco;
			class Poligono;
		}}}

using namespace simulacao::model::interceptos;

namespace simulacao{
	namespace canvas{
		namespace drawVisitor{

			class DrawVisitor{
			public:
				DrawVisitor();
				virtual void visit(Disco *disco) = 0;
				virtual void visit(Poligono *poligono) = 0;
			};

		}
	}
}

#endif