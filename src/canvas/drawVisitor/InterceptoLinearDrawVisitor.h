#ifndef INTERCEPTO_LINEAR_DRAW_VISITOR
#define INTERCEPTO_LINEAR_DRAW_VISITOR

#include <list>
#include "AbstractDrawVisitor.h"
#include "..\..\model\grade\Grade.h"
#include "..\..\model\grade\RetaDeTeste.h"
#include "..\..\utils\SegmentoDeReta.h"

using namespace simulacao::model::grade;
using std::list;

namespace simulacao{
	namespace canvas{
		namespace drawVisitor{

			class InterceptoLinearDrawVisitor : public AbstractDrawVisitor{
			private:
				Grade *grade;

				inline list<SegmentoDeReta> selecionarArestasInterceptadas(list<SegmentoDeReta> &arestas,double zLinha);
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