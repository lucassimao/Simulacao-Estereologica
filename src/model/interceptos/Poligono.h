#ifndef POLIGONO_H
#define POLIGONO_H

#include <list>
#include <vector>
#include "InterceptoDeArea.h"
#include "..\grade\RetaDeTeste.h"
#include "..\..\defs.h"
#include "..\..\utils\SegmentoDeReta.h"


using std::vector;
using std::list;
using namespace simulacao::model::grade;

namespace simulacao{ 
	namespace model {
		namespace interceptos{

			class Poligono: public InterceptoDeArea{	
			private:
				inline list<SegmentoDeReta> coletarArestas();
				list<Ponto> vertices;
				list<SegmentoDeReta> arestas;
				inline Ponto procurarVerticeComMenorZ();
				inline Ponto procurarVerticeComMaiorZ();
			public:
				Ponto verticeComMenorZ;
				Ponto verticeComMaiorZ;

				Poligono(Cor cor,list<Ponto> vertices,double razaoDeAspectoOriginal,double razaoDeTruncamentoOriginal,double L0Original);
				

				void accept(AbstractDrawVisitor *);
				void accept(AbstractMathVisitor *);				
				double getArea();
				double getPerimetro();
				const double razaoDeAspectoOriginal, razaoDeTruncamentoOriginal, L0Original;

				list<Ponto> getVertices() const{ return this->vertices; };
				list<SegmentoDeReta> getArestas(){ return this->arestas;};
				list<SegmentoDeReta> getArestasInterceptadas(RetaDeTeste&);
				vector<InterceptoLinear*> getInterceptosLineares(Grade *grade);
				
				InterceptoType getType(){ return Type_Poligono;	}
				
			};

		}
	}
}

#endif