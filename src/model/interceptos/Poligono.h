#ifndef POLIGONO_H
#define POLIGONO_H

#include <list>
#include <vector>
#include "Intercepto.h"
#include "..\..\defs.h"
#include "..\..\utils\SegmentoDeReta.h"

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
				inline Ponto procurarVerticeComMenorZ();
				inline Ponto procurarVerticeComMaiorZ();
				inline list<SegmentoDeReta> coletarArestas();

				list<Ponto> vertices;
				list<SegmentoDeReta> arestas;
			public:
				Poligono(Cor cor,list<Ponto> vertices);
				
				Ponto verticeComMenorZ;
				Ponto verticeComMaiorZ;

				void accept(AbstractDrawVisitor *);
				list<Ponto> getVertices() const{ return this->vertices; };
				list<SegmentoDeReta> getArestas(){ return this->arestas;};
				
			};

		}
	}
}

#endif