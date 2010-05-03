#ifndef DISCO_H
#define DISCO_H

#include "Intercepto.h"
#include "..\..\defs.h"

using std::vector;

namespace simulacao{ 
	namespace model {
		namespace interceptos{

			class Disco: public Intercepto{		
			public:
				const Ponto centro;
				const double raio;
				const double raioDaEsferaDeOrigem;
				
				Disco(Cor cor,Ponto centro,double raio, double raioDaEsferaDeOrigem);
				Disco(Ponto centro,double raio, double raioDaEsferaDeOrigem);

				double getArea();
				double getPerimetro();
				void accept(AbstractDrawVisitor *visitor);
				void accept(AbstractMathVisitor *);				

			};

		}
	}
}
#endif