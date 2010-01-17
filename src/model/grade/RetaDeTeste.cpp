#include <cmath>
#include "RetaDeTeste.h"

using namespace simulacao::model::grade;

RetaDeTeste::RetaDeTeste(Ponto p0, Ponto p1, int qtdeDePontos){
	this->linhaInicio = p0;
	this->linhaFim = p1;
	double pontoFim = linhaFim.x-0.5;
	double pontoInicio = linhaInicio.x + 0.5;
	double deltaX = abs(pontoFim - pontoInicio)/qtdeDePontos;
			
	for(int i=0;i<qtdeDePontos;++i){
		Ponto p = {pontoInicio +  deltaX*i,linhaInicio.y,linhaInicio.z};
		pontos.push_back(p);
	}

}