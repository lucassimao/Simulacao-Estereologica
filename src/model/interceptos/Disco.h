#ifndef DISCO_H
#define DISCO_H

#include "InterceptoDeArea.h"
#include "..\..\defs.h"

using std::vector;

namespace simulacao{ 
	namespace model {
		namespace interceptos{

			class Disco: public InterceptoDeArea{		
			public:
				const int atorID;
				const Ponto centro;
				const double raio;
				const double raioDaEsferaDeOrigem;
				
				Disco(Cor cor,Ponto centro,double raio, double raioDaEsferaDeOrigem,int atorID);
				Disco(Ponto centro,double raio, double raioDaEsferaDeOrigem,int atorID);

				double getArea();
				double getPerimetro();
				void accept(AbstractDrawVisitor *visitor);
				void accept(AbstractMathVisitor *);		
				vector<InterceptoLinear*> getInterceptosLineares(Grade *grade);
				InterceptoType getType(){ return Type_Disco;	}

			};

		}
	}
}
#endif