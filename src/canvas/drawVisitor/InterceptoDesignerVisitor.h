#ifndef INTERCEPTO_DESIGNER_VISITOR_H
#define INTERCEPTO_DESIGNER_VISITOR_H

#include "DrawVisitor.h"

namespace simulacao{
	namespace canvas{
		namespace drawVisitor{

			class InterceptoDesignerVisitor : public DrawVisitor{
			public:
				InterceptoDesignerVisitor();
				void visit(Disco *disco);
				void visit(Poligono *poligono);
			};

		}
	}
}

#endif