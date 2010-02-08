#include "Parametros.h"

using namespace simulacao::model;

Parametros* Parametros::instance = 0;

Parametros::Parametros(){
	this->raioEsfera =0.5;
	this->arestaCubo = 1.0;
	prismaTriangular.altura=2.0;
	prismaTriangular.base=1.5;
	prismaTriangularTruncado.razaoDeAspecto=1;
	prismaTriangularTruncado.razaoDeTruncamento=0.3;
	prismaTriangularTruncado.L0 = 5;
	this->arestaDaCaixa = 20.0;
}