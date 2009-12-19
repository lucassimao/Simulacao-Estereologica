#ifndef POLIGONO_H
#define POLIGONO_H

#include <vector>
#include <list>
#include "Intercepto.h"
#include "..\..\defs.h"

using std::vector;
using std::list;

namespace simulacao{ 
	namespace model {
		namespace interceptos{

			class Poligono: public Intercepto{	
			private:
				inline void ordenarVertices();
				inline Ponto getVerticeMaisAEsquerda();
				inline list<Ponto> procurarVerticesAcimaDoVerticeMaisAEsquerda(Ponto&);
				inline list<Ponto> procurarVerticesAbaixoDoVerticeMaisAEsquerda(Ponto&);
				list<Ponto> vertices;
			public:
				Poligono(Cor cor,list<Ponto> vertices);
				void accept(DrawVisitor *);
				list<Ponto> getVertices() const{ return this->vertices; };
				
			};

		}
	}
}

#endif