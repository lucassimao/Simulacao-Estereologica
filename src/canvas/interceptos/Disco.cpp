#include "Disco.h"


using namespace simulacao::canvas::interceptos;

Disco::Disco(Cor cor,Ponto centro,double raio):Intercepto(cor){
	this->raio = raio;
	this->centro = centro;
}

Disco::Disco(Ponto centro,double raio):Intercepto(){
	this->raio = raio;
	this->centro = centro;
}

DrawCommand* Disco::getDrawCommand() const{
	return NULL;
}
