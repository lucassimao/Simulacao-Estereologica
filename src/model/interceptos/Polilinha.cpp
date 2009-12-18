#include "Polilinha.h"
using namespace simulacao::model::interceptos;

#include <vector>
using std::vector;

#include "..\..\canvas\drawVisitor\DrawVisitor.h"
using namespace simulacao::canvas::drawVisitor;


Polilinha::Polilinha(Cor cor,vector<SegmentoDeReta> vertices):Intercepto(cor){
	this->vertices = vertices;
}

void Polilinha::accept(DrawVisitor *visitor){
	visitor->visit(this);
}
