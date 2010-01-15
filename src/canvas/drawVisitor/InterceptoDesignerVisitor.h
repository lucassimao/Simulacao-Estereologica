#ifndef INTERCEPTO_DESIGNER_VISITOR_H
#define INTERCEPTO_DESIGNER_VISITOR_H

#include "AbstractDrawVisitor.h"

namespace simulacao{
	namespace canvas{
		namespace drawVisitor{

			class InterceptoDesignerVisitor : public AbstractDrawVisitor{
			public:
				InterceptoDesignerVisitor();
				inline void visit(Disco *disco);
				inline void visit(Poligono *poligono);
				inline void visit(Polilinha *poligono);
			};

		}
	}
}

#endif