#ifndef INTERCEPTO_DESIGNER_VISITOR_H
#define INTERCEPTO_DESIGNER_VISITOR_H

#include "AbstractDrawVisitor.h"

namespace simulacao{
	namespace canvas{
		namespace drawVisitor{

			class InterceptoDeAreaDrawVisitor : public AbstractDrawVisitor{
			public:
				InterceptoDeAreaDrawVisitor();
				inline void visit(Disco *disco);
				inline void visit(Poligono *poligono);
		};

		}
	}
}

#endif