#include "AbstractGeradorDeAlturaDoPlanoDeCorteStrategy.h"
#include <cstdlib>
#include <ctime>
using namespace simulacao::utils;

AbstractGeradorDeAlturaDoPlanoDeCorteStrategy::AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(float h0,float h1){
	this->h0 = h0;
	this->h1 = h1;
	srand(time(0));
}