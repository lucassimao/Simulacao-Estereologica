#include "Disco.h"
using namespace simulacao::model::interceptos;

#include "..\..\canvas\drawVisitor\AbstractDrawVisitor.h"
using namespace simulacao::canvas::drawVisitor;

Disco::Disco(Cor cor,Ponto centro,double raio):Intercepto(cor){
	this->raio = raio;
	this->centro = centro;
}

Disco::Disco(Ponto centro,double raio):Intercepto(){
	this->raio = raio;
	this->centro = centro;
}

void Disco::accept(AbstractDrawVisitor *visitor){
	visitor->visit(this);
}

bool Disco::contemPonto(Ponto p){
	double distanciaEmRelacaoAoCentro = pow(pow(p.x-this->centro.x,2) + pow(p.z - this->centro.z,2),0.5);
	return (distanciaEmRelacaoAoCentro <= this->raio) ;
}
