#ifndef POLIGONO_H
#define POLIGONO_H

#include <vector>
#include "Intercepto.h"
#include "designer\DrawCommand.h"
#include "..\..\defs.h"

using std::vector;
using namespace simulacao::canvas::interceptos::designer;

namespace simulacao{ 
	namespace canvas {
		namespace interceptos{

			class Poligono: public Intercepto{
			private:
				vector<Ponto> vertices;
			public:
				Poligono(Cor cor,vector<Ponto> vertices);
				DrawCommand* getDrawCommand();
				
			};

		}
	}
}

#endif