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
				
				Disco(Cor cor,Ponto centro,double raio);
				Disco(Ponto centro,double raio);

				double getArea();
				double getPerimetro();
				void accept(AbstractDrawVisitor *visitor);
				void accept(AbstractMathVisitor *);				

			};

		}
	}
}
#endif