#include <cstdlib>
#include <cmath>
#include "GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h"

using namespace simulacao::utils;

GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy::GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy(float h0,float h1):AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(h0,h1)
{}
float GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy::novaAltura(){
	return this->h0 + fmod(rand(),(this->h1 - this->h0) );
}