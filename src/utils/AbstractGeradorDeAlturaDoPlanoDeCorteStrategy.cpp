#include "AbstractGeradorDeAlturaDoPlanoDeCorteStrategy.h"
#include <cstdlib>
#include <ctime>
using namespace simulacao::utils;

AbstractGeradorDeAlturaDoPlanoDeCorteStrategy::AbstractGeradorDeAlturaDoPlanoDeCorteStrategy(double h0,double h1){
	this->h0 = h0;
	this->h1 = h1;
	srand(time(0));
}