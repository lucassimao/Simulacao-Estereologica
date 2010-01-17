#include "Disco.h"
using namespace simulacao::model::interceptos;

#include "..\..\canvas\drawVisitor\AbstractDrawVisitor.h"
using namespace simulacao::canvas::drawVisitor;

Disco::Disco(Cor cor,Ponto c,double r):Intercepto(cor),raio(r),centro(c){
}

Disco::Disco(Ponto c,double r):Intercepto(),raio(r),centro(c){
}

void Disco::accept(AbstractDrawVisitor *visitor){
	visitor->visit(this);
}

bool Disco::contemPonto(Ponto p){
	double distanciaEmRelacaoAoCentro = pow(pow(p.x-this->centro.x,2) + pow(p.z - this->centro.z,2),0.5);
	return (distanciaEmRelacaoAoCentro <= this->raio) ;
}
