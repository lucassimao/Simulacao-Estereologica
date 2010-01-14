#ifndef DISCO_H
#define DISCO_H

#include "Intercepto.h"
#include "..\..\defs.h"

using std::vector;

namespace simulacao{ 
	namespace model {
		namespace interceptos{

			class Disco: public Intercepto{
			private:
				Ponto centro;
				double raio;
			public:
				Disco(Cor cor,Ponto centro,double raio);
				Disco(Ponto centro,double raio);
				double getRaio() const { return raio;} ;
				Ponto getCentro() const { return centro;};
				void accept(DrawVisitor *visitor);
				bool contemPonto(Ponto p);
			};

		}
	}
}
#endif