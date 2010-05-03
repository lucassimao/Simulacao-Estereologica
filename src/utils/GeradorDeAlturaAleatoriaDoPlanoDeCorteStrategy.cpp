#include <cstdlib>
#include <cmath>
#include "..\model\Parametros.h"
#include "GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h"

using namespace simulacao::utils;
using namespace simulacao::model;


Parametros *params = Parametros::getInstance();
double hMinima = params->getAlturaDaBaseDaCaixa();
double hMaxima = params->getArestaDaCaixa() + params->getAlturaDaBaseDaCaixa();

GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy::GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy()
	:AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(hMinima,hMaxima)
{}

GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy::GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy(double h0,double h1):AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(h0,h1)
{}
double GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy::novaAltura(){
	return this->h0 + fmod(rand(),(this->h1 - this->h0) );
}