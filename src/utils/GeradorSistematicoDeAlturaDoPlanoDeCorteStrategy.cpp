#include "GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy.h"
#include "..\model\Parametros.h"

using namespace simulacao::utils;
using namespace simulacao::model;

Parametros *p = Parametros::getInstance();
double alturaMinimaPlanoDeCorte = p->getAlturaDaBaseDaCaixa();
double alturaMaximaPlanoDeCorte = p->getArestaDaCaixa() + p->getAlturaDaBaseDaCaixa();

GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy::GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy(int qtdePlanos): 
AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(alturaMinimaPlanoDeCorte,alturaMaximaPlanoDeCorte){
	this->deltaH = (alturaMaximaPlanoDeCorte - alturaMinimaPlanoDeCorte)/(qtdePlanos+1.0);
	this->alturaCorrente = h0 + deltaH;
}


GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy::GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy(double h0,double h1,int qtdePlanos): AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(h0,h1){
	this->deltaH = (h1 - h0)/(qtdePlanos+1.0);
	this->alturaCorrente = h0+deltaH;
}

double GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy::novaAltura(){
	if ( (alturaCorrente+deltaH) > h1)
		this->alturaCorrente = h0+deltaH;

	double altura =alturaCorrente;
	this->alturaCorrente += deltaH;

	return altura;
}