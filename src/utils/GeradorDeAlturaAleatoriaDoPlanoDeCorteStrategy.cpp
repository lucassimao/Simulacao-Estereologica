#include <cstdlib>
#include <cmath>
#include "..\model\Parametros.h"
#include "GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h"

using namespace simulacao::utils;
using namespace simulacao::model;


GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy::GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy()
:AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(0,0)
{}

GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy::GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy(double h0,double h1):AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(h0,h1)
{}
double GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy::novaAltura(){
	Parametros *params = Parametros::getInstance();
	this->h0 = params->getAlturaDaBaseDaCaixa();
	this->h1 = params->getArestaDaCaixa() + params->getAlturaDaBaseDaCaixa();
	return this->h0 + fmod(rand(),(this->h1 - this->h0) );
}