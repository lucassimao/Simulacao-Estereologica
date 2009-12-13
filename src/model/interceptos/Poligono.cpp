#include "Poligono.h"
using namespace simulacao::model::interceptos;

#include <vector>
using std::vector;

#include "..\..\canvas\drawVisitor\DrawVisitor.h"
using namespace simulacao::canvas::drawVisitor;


Poligono::Poligono(Cor cor,vector<Ponto> vertices):Intercepto(cor){
	this->vertices = vertices;
}

void Poligono::accept(DrawVisitor *visitor){
	visitor->visit(this);
}
