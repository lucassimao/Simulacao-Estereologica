#ifndef POLIGONO_H
#define POLIGONO_H

#include <vector>
#include "Intercepto.h"
#include "..\..\defs.h"

using std::vector;

namespace simulacao{ 
	namespace model {
		namespace interceptos{

			class Poligono: public Intercepto{
			private:
				vector<Ponto> vertices;
			public:
				Poligono(Cor cor,vector<Ponto> vertices);
				void accept(DrawVisitor *);
				
			};

		}
	}
}

#endif