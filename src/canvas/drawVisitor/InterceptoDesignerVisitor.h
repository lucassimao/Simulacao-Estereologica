#ifndef INTERCEPTO_DESIGNER_VISITOR_H
#define INTERCEPTO_DESIGNER_VISITOR_H

#include "DrawVisitor.h"

namespace simulacao{
	namespace canvas{
		namespace drawVisitor{

			class InterceptoDesignerVisitor : public DrawVisitor{
			public:
				InterceptoDesignerVisitor();
				inline void visit(Disco *disco);
				inline void visit(Poligono *poligono);
			};

		}
	}
}

#endif