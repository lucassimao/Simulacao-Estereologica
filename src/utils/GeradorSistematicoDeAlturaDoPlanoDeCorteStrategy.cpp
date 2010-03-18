#include "GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy.h"

using namespace simulacao::utils;

GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy::GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy(float h0,float h1,int qtdePlanos): AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(h0,h1){
	this->deltaH = (h1 - h0)/qtdePlanos;
	this->alturaCorrente = h0 + deltaH;
}

float GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy::novaAltura(){
	if (alturaCorrente > h1)
		this->alturaCorrente = h0 + deltaH;
	else 
		this->alturaCorrente += deltaH;

	return this->alturaCorrente;
}