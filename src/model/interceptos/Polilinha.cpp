#include "Polilinha.h"
using namespace simulacao::model::interceptos;

#include <vector>
using std::vector;

#include "..\..\canvas\drawVisitor\AbstractDrawVisitor.h"
using namespace simulacao::canvas::drawVisitor;


Polilinha::Polilinha(Cor cor,vector<SegmentoDeReta> vertices):Intercepto(cor){
	this->vertices = vertices;
}

void Polilinha::accept(AbstractDrawVisitor *visitor){
	visitor->visit(this);
}
double Polilinha::calcularArea(){
	return 0.0;
}
