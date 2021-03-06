#include "Poligono.h"
using namespace simulacao::model::interceptos;

#include <vector>
using std::vector;

#include <list>
using std::list;

#include "..\..\canvas\drawVisitor\AbstractDrawVisitor.h"
using namespace simulacao::canvas::drawVisitor;

#include "..\..\math\AbstractMathVisitor.h"
using namespace simulacao::math::mathVisitor;


#include "..\..\utils\Vetor.h"
#include "..\..\utils\jarvis.h"
#include "..\..\utils\SegmentoDeReta.h"

#include <Nxvec3.h>

#define MAX(a,b) ( (a>=b)?a:b )
#define MIN(a,b) ( (a<=b)?a:b )



Poligono::Poligono(Cor cor,list<Ponto> v,double razaoDeAspectoOriginal,double razaoDeTruncamentoOriginal,double L0Original,int atorID):
InterceptoDeArea(cor),razaoDeAspectoOriginal( razaoDeAspectoOriginal ),	razaoDeTruncamentoOriginal( razaoDeTruncamentoOriginal ),L0Original( L0Original ) 
{
	assert(v.size() >= 3 );
	if (v.size()>3)
		this->vertices = ordenarVertices(v);
	else 
		this->vertices = v;

	assert(this->vertices.size()>=3);
	
	this->atorID = atorID;
	this->verticeComMaiorZ = procurarVerticeComMaiorZ();
	this->verticeComMenorZ = procurarVerticeComMenorZ();	
	this->arestas = coletarArestas();	

}

double Poligono::getPerimetro(){
	double perimetro = 0;

	list<SegmentoDeReta>::const_iterator segmentos = this->arestas.begin();
	
	while(segmentos!= this->arestas.end()){
		SegmentoDeReta s= *segmentos;
	
		NxVec3 vec0(s.r0.x,s.r0.y,s.r0.z);
		NxVec3 vec1(s.r1.x,s.r1.y,s.r1.z);

		NxVec3 vetorEquivalente = vec1 - vec0;
		perimetro += vetorEquivalente.magnitude();
		segmentos++;
	}
	
	return perimetro;
}

inline Ponto Poligono::procurarVerticeComMenorZ(){
	Ponto vertice = vertices.front();

	list<Ponto>::const_iterator iter = vertices.begin();
	
	while(iter!= vertices.end()){
		Ponto p = *iter;
		if (p.z < vertice.z)
			vertice = p;
		++iter;
	}
	
	return vertice;
}

inline Ponto Poligono::procurarVerticeComMaiorZ(){
	Ponto vertice = vertices.front();
	list<Ponto>::const_iterator iter = vertices.begin();
	
	while(iter!= vertices.end()){
		Ponto p = *iter;
		if (p.z > vertice.z)
			vertice = p;
		++iter;
	}
	

	return vertice;
}

vector<InterceptoLinear*> Poligono::getInterceptosLineares(Grade *grade){
	vector<InterceptoLinear*> interceptosLineares;

	double z0 = this->verticeComMaiorZ.z;
	double z1 = this->verticeComMenorZ.z;
	
	vector<RetaDeTeste> linhas = grade->getLinhasNoIntervalo(z0,z1);
	vector<RetaDeTeste>::const_iterator iterator = linhas.begin();
	
	while(iterator!=linhas.end()){
			RetaDeTeste retaDeTeste = *iterator;

			list<SegmentoDeReta> arestasInterceptadas = this->getArestasInterceptadas(retaDeTeste);
			assert( arestasInterceptadas.size()==2 );

			SegmentoDeReta seg1 = arestasInterceptadas.front();
			Ponto p1;
			seg1.interceptar(retaDeTeste,&p1);
			
			SegmentoDeReta seg2 = arestasInterceptadas.back();
			Ponto p2;
			seg2.interceptar(retaDeTeste,&p2);

			if (p1.x < p2.x)			
				interceptosLineares.push_back(new InterceptoLinear(p1,p2));	
			else
				interceptosLineares.push_back(new InterceptoLinear(p2,p1));	

			++iterator;
	}
	return interceptosLineares;
}

double Poligono::getArea(){
	double area = 0.0;

	list<Ponto>::const_iterator iter = vertices.begin();
	list<Ponto>::const_iterator fim = vertices.end();

	Ponto p0 = *iter;
	++iter;
	
	while(true){
		Ponto p1 = *iter;
		++iter;
		
		if (iter==fim){
			break;
		}
		Ponto p2 = *iter;

		NxVec3 v1(p1.x-p0.x,p1.y-p0.y,p1.z-p0.z);
		NxVec3 v2(p2.x-p0.x,p2.y-p0.y,p2.z-p0.z);
		area += 0.5 * v1.cross(v2).magnitude();	
	}

	return area;
}

list<SegmentoDeReta> Poligono::getArestasInterceptadas(RetaDeTeste& rt){
	double zLinha = rt.linhaInicio.z;
	double xLinha0 = rt.linhaInicio.x;
	double xLinha1 = rt.linhaFim.x;

	list<SegmentoDeReta> arestasInterceptadas;
	list<SegmentoDeReta>::iterator iter = arestas.begin();
	
	while(iter!= arestas.end()){
		SegmentoDeReta s = *iter;
		Vetor v0 = s.r0;
		Vetor v1 = s.r1;

		if (  (zLinha <= MAX(v0.z,v1.z)) && ( zLinha >= MIN(v0.z,v1.z)))
			arestasInterceptadas.push_back(s);

		++iter;
	}
	return arestasInterceptadas;
}


inline list<SegmentoDeReta> Poligono::coletarArestas(){
	
	list<SegmentoDeReta> arestas;

	list<Ponto>::const_iterator iter = vertices.begin();
	Ponto vertice0 = *iter;
	++iter;

	while(iter!= vertices.end()){
		Ponto p = *iter;
		arestas.push_back(SegmentoDeReta(vertice0,p));
		vertice0 = p;
		++iter;
	}	
	Ponto ultimoVertice = vertices.back();
	Ponto primeiroVertice = vertices.front();
	arestas.push_back(SegmentoDeReta(ultimoVertice,primeiroVertice));
	return arestas;
	
}

void Poligono::accept(AbstractDrawVisitor *visitor){
	
	visitor->visit(this);
	
}
void Poligono::accept(AbstractMathVisitor *visitor){
	visitor->visit(this);
}