#include <cmath>
#include <cassert>
#include <stdexcept>
#include "RetaDeTeste.h"

using namespace simulacao::model::grade;
using  std::exception;

RetaDeTeste::RetaDeTeste(Ponto p0, Ponto p1, int qtdeDePontos){
	assert(p0.y == p1.y);
	assert(p0.z == p1.z);

	this->linhaInicio = p0;
	this->linhaFim = p1;

	double pontoInicio = linhaInicio.x;
	double deltaX =  ( abs(p0.x) + abs(p1.x))/(qtdeDePontos + 1.0);

	for(int i=1;i<=qtdeDePontos;++i){
		Ponto p = {pontoInicio +  deltaX*i,linhaInicio.y,linhaInicio.z};
		pontos.push_back(p);
	}
	assert(pontos.size() == qtdeDePontos);

}

int RetaDeTeste::getQtdeDePontosNoIntervalo(Ponto p0,Ponto p1){
	assert(( (p0.y == p1.y) && (p1.y == this->linhaInicio.y) && (p0.z == p1.z) && (p1.z == this->linhaInicio.z) ));

	int qtde = 0;

	vector<Ponto>::const_iterator iterator = this->pontos.begin();
	while(iterator != this->pontos.end()){
		Ponto p = *iterator;

		if (p.x >= p0.x && p.x <= p1.x)
			++qtde;

		iterator++;
	}



	return qtde;
}
