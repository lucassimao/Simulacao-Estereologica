#include "Disco.h"
using namespace simulacao::model::interceptos;

#include "..\..\canvas\drawVisitor\DrawVisitor.h"
using namespace simulacao::canvas::drawVisitor;

Disco::Disco(Cor cor,Ponto centro,double raio):Intercepto(cor){
	this->raio = raio;
	this->centro = centro;
}

Disco::Disco(Ponto centro,double raio):Intercepto(){
	this->raio = raio;
	this->centro = centro;
}

void Disco::accept(DrawVisitor *visitor){
	visitor->visit(this);
}

