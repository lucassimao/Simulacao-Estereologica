#include <cstdlib>
#include <cmath>
#include "GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h"

using namespace simulacao::utils;

float GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy::novaAltura(){
	return (this->h0 + fmod(rand(), this->h1) );
}