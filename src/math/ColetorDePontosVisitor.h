#ifndef COLETOR_DE_PONTOS_VISITOR_H
#define COLETOR_DE_PONTOS_VISITOR_H

#include <vector>
#include "AbstractMathVisitor.h"
#include "..\model\interceptos\Poligono.h"
#include "..\model\interceptos\Disco.h"
#include "..\model\grade\Grade.h"
#include "..\model\grade\RetaDeTeste.h"

using std::vector;
using namespace simulacao::model::grade;
using namespace simulacao::model::interceptos;

namespace simulacao{
	namespace math{
		namespace mathVisitor{

			class ColetorDePontosVisitor : public AbstractMathVisitor{
			private:
				Grade *grade;
				int qtdePontos;
			public:
				ColetorDePontosVisitor(Grade *);

				inline void visit(Disco *disco);
				inline void visit(Poligono *poligono);

				int getQtdeDePontosInternosAInterceptosDeArea(){ return this->qtdePontos;}
			};

		}
	}
}

#endif