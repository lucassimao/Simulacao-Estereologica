#include "Disco.h"


using namespace simulacao::model::interceptos;

Disco::Disco(Cor cor,Ponto centro,double raio):Intercepto(cor){
	this->raio = raio;
	this->centro = centro;
}

Disco::Disco(Ponto centro,double raio):Intercepto(){
	this->raio = raio;
	this->centro = centro;
}

