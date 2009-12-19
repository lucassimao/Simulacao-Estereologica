#include "Poligono.h"
using namespace simulacao::model::interceptos;

#include <vector>
using std::vector;

#include <list>
using std::list;

#include "..\..\canvas\drawVisitor\DrawVisitor.h"
using namespace simulacao::canvas::drawVisitor;


Poligono::Poligono(Cor cor,list<Ponto> vertices):Intercepto(cor){
	this->vertices = vertices;
	if (vertices.size()>3)
		ordenarVertices();
}

void Poligono::accept(DrawVisitor *visitor){
	visitor->visit(this);
}

inline bool ordenarPontosAcimaDoMaisAEsquerda(Ponto p1,Ponto p2){
	return (p1.x < p2.x);
}

inline bool ordenarPontosAbaixoDoMaisAEsquerda(Ponto p1,Ponto p2){
	return (p1.x > p2.x);
}

inline void Poligono::ordenarVertices(){
	Ponto verticeMaisAEsquerda = getVerticeMaisAEsquerda();
    
	list<Ponto> pontosAcimaDoPontoMaisAEsquerda = procurarVerticesAcimaDoVerticeMaisAEsquerda(verticeMaisAEsquerda);
	pontosAcimaDoPontoMaisAEsquerda.sort(ordenarPontosAcimaDoMaisAEsquerda);
    
	list<Ponto> pontosAbaixoDoPontoMaisAEsquerda = procurarVerticesAbaixoDoVerticeMaisAEsquerda(verticeMaisAEsquerda);
	pontosAbaixoDoPontoMaisAEsquerda.sort(ordenarPontosAbaixoDoMaisAEsquerda);

	this->vertices.clear();
	this->vertices.push_front(verticeMaisAEsquerda);
	
	list<Ponto>::const_iterator iter = pontosAcimaDoPontoMaisAEsquerda.begin();
	while(iter!=pontosAcimaDoPontoMaisAEsquerda.end())
	{
		Ponto p = *iter;
		this->vertices.push_back(p);
		++iter;
	}

	list<Ponto>::const_iterator iter2 = pontosAbaixoDoPontoMaisAEsquerda.begin();
	while(iter2!=pontosAbaixoDoPontoMaisAEsquerda.end())
	{
		Ponto p = *iter2;
		this->vertices.push_back(p);
		++iter2;
	}
}



inline Ponto Poligono::getVerticeMaisAEsquerda(){
	Ponto vertice;
	list<Ponto>::const_iterator verticePos;
	register int index=0;
	list<Ponto>::const_iterator i = this->vertices.begin();

	while(i!=this->vertices.end()){
		Ponto p = *i;
		if (index == 0 || (p.x < vertice.x) ){
			vertice = p;
			verticePos = i;
		}
		++i;
		++index;
	}
	this->vertices.erase(verticePos);
	return vertice;
}

inline list<Ponto> Poligono::procurarVerticesAcimaDoVerticeMaisAEsquerda(Ponto &verticeMaisAEsquerda){
	list<Ponto> verticesAcimaDoVerticeMaisAEsquerda;

	list<Ponto>::const_iterator i = this->vertices.begin();
	while(i!=this->vertices.end()){
		Ponto p = *i;
		if (p.z <= verticeMaisAEsquerda.z )
			verticesAcimaDoVerticeMaisAEsquerda.push_back(p);
		++i;
	}
	return verticesAcimaDoVerticeMaisAEsquerda;
}

inline list<Ponto> Poligono::procurarVerticesAbaixoDoVerticeMaisAEsquerda(Ponto &verticeMaisAEsquerda){
	list<Ponto> verticesAbaixoDoVerticeMaisAEsquerda;
	list<Ponto>::const_iterator i = this->vertices.begin();

	while(i!=this->vertices.end()){
		Ponto p = *i;
		if (p.z > verticeMaisAEsquerda.z )
			verticesAbaixoDoVerticeMaisAEsquerda.push_back(p);
		++i;
	}
	return verticesAbaixoDoVerticeMaisAEsquerda;
}