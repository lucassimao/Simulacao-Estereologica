#include "Parametros.h"
#include "..\defs.h"

using namespace simulacao::model;

Parametros* Parametros::instance = 0;

Parametros::Parametros(){
	Cor c = VERMELHO;
	
	this->esfera.raio=0.8;
	this->esfera.cor = c;

	prismaTriangular.altura=2.0;
	prismaTriangular.base=1.5;
	prismaTriangular.cor = c;
	
	prismaTriangularTruncado.razaoDeAspecto=0.3;
	prismaTriangularTruncado.razaoDeTruncamento=0.3;
	prismaTriangularTruncado.L0 = 2.5;
	prismaTriangularTruncado.cor = c;
	
	this->arestaDaCaixa = 20.0;
	this->grade.pontosAleatorios=false;
	this->grade.qtdeLinhas=20;
	this->grade.qtdePontos=10;
}