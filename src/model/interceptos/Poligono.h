#ifndef POLIGONO_H
#define POLIGONO_H

#include <list>
#include <vector>
#include "Intercepto.h"
#include "..\grade\RetaDeTeste.h"
#include "..\..\defs.h"
#include "..\..\utils\SegmentoDeReta.h"


using std::vector;
using std::list;
using namespace simulacao::model::grade;

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
				Ponto verticeComMenorZ;
				Ponto verticeComMaiorZ;

				Poligono(Cor cor,list<Ponto> vertices);
				

				void accept(AbstractDrawVisitor *);
				void accept(AbstractMathVisitor *);				
				double getArea();
				double getPerimetro();

				list<Ponto> getVertices() const{ return this->vertices; };
				list<SegmentoDeReta> getArestas(){ return this->arestas;};
				list<SegmentoDeReta> getArestasInterceptadas(RetaDeTeste&);
				
			};

		}
	}
}

#endif