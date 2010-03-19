#include <cstdlib>
#include <cmath>
#include "..\model\Parametros.h"
#include "GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h"

using namespace simulacao::utils;
using namespace simulacao::model;


Parametros *params = Parametros::getInstance();
float hMinima = params->getAlturaDaBaseDaCaixa()+0.5f;
float hMaxima = params->getArestaDaCaixa() + params->getAlturaDaBaseDaCaixa() - 0.5f;

GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy::GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy()
	:AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(hMinima,hMaxima)
{}

GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy::GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy(float h0,float h1):AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(h0,h1)
{}
float GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy::novaAltura(){
	return this->h0 + fmod(rand(),(this->h1 - this->h0) );
}