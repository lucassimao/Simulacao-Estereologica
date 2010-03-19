#include "GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy.h"
#include "..\model\Parametros.h"

using namespace simulacao::utils;
using namespace simulacao::model;

Parametros *p = Parametros::getInstance();
float alturaMinimaPlanoDeCorte = p->getAlturaDaBaseDaCaixa()+0.5f;
float alturaMaximaPlanoDeCorte = p->getArestaDaCaixa() + p->getAlturaDaBaseDaCaixa() - 0.5f;

GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy::GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy(int qtdePlanos): AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(alturaMinimaPlanoDeCorte,alturaMaximaPlanoDeCorte){
	this->deltaH = (alturaMaximaPlanoDeCorte - alturaMinimaPlanoDeCorte)/(qtdePlanos+1);
	this->alturaCorrente = h0 + deltaH;
}


GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy::GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy(float h0,float h1,int qtdePlanos): AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(h0,h1){
	this->deltaH = (h1 - h0)/(qtdePlanos+1);
	this->alturaCorrente = h0+deltaH;
}

float GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy::novaAltura(){
	if ( (alturaCorrente+deltaH) > h1)
		this->alturaCorrente = h0+deltaH;

	float altura =alturaCorrente;
	this->alturaCorrente += deltaH;

	return altura;
}