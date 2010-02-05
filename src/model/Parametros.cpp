#include "Parametros.h"

using namespace simulacao::model;

Parametros* Parametros::instance = 0;

Parametros::Parametros(){
	this->raioEsfera = 1.0;
	this->arestaCubo = 1.0;
	this->alturaPrisma=5.0;
	this->basePrisma=2.0;
}