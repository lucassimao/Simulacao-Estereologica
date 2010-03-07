#include "Parametros.h"

using namespace simulacao::model;

Parametros* Parametros::instance = 0;

Parametros::Parametros(){
	this->raioEsfera =0.8;
	this->arestaCubo = 1.0;
	prismaTriangular.altura=2.0;
	prismaTriangular.base=1.5;
	prismaTriangularTruncado.razaoDeAspecto=0.3;
	prismaTriangularTruncado.razaoDeTruncamento=0.3;
	prismaTriangularTruncado.L0 = 2.5;
	this->arestaDaCaixa = 20.0;
	this->grade.pontosAleatorios=false;
	this->grade.qtdeLinhas=20;
	this->grade.qtdePontos=10;
}