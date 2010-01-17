#ifndef INTERCEPTO_LINEAR_DRAW_VISITOR
#define INTERCEPTO_LINEAR_DRAW_VISITOR


#include "AbstractDrawVisitor.h"
#include "..\..\model\grade\Grade.h"
#include "..\..\model\grade\Linha.h"

using namespace simulacao::model::grade;

namespace simulacao{
	namespace canvas{
		namespace drawVisitor{

			class InterceptoLinearDrawVisitor : public AbstractDrawVisitor{
			private:
				Grade *grade;
			public:
				InterceptoLinearDrawVisitor(Grade *);
				inline void visit(Disco *disco);
				inline void visit(Poligono *poligono);
				inline void visit(Polilinha *poligono);
			};

		}
	}
}

#endif