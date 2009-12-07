#ifndef DISCO_H
#define DISCO_H

#include "Intercepto.h"
#include "designer\DrawCommand.h"
#include "..\..\defs.h"

using std::vector;
using namespace simulacao::canvas::interceptos::designer;

namespace simulacao{ 
	namespace canvas {
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
				DrawCommand* getDrawCommand() const;
			};

		}
	}
}
#endif