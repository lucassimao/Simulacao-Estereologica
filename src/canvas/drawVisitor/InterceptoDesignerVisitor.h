#ifndef INTERCEPTO_DESIGNER_VISITOR_H
#define INTERCEPTO_DESIGNER_VISITOR_H

#include "DrawVisitor.h"

namespace simulacao{
	namespace canvas{
		namespace drawVisitor{

			class InterceptoDesignerVisitor : public DrawVisitor{
			public:
				InterceptoDesignerVisitor();
				virtual void visit(Disco *disco);
				virtual void visit(Poligono *poligono);
			};

		}
	}
}

#endif