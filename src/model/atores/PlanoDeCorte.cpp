#include "PlanoDeCorte.h"

using namespace simulacao::model::atores;

PlanoDeCorte::PlanoDeCorte(NxActor * nxActor):Ator()
{	
	this->ator = nxActor;
	this->altura = nxActor->getGlobalPosition().y;
	this->grade = 0;
}



void PlanoDeCorte::update(Observable* obs){
	int qtdeLinha = Parametros::getInstance()->getParametrosDaGrade().qtdeLinhas; 
	int qtdePontos = Parametros::getInstance()->getParametrosDaGrade().qtdePontos;

	this->grade = new Grade(9,-9,altura,qtdeLinha,qtdePontos);
}

bool PlanoDeCorte::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	return false;
}

Grade *PlanoDeCorte::getGrade(){
	if (!grade){
		int qtdeLinha = Parametros::getInstance()->getParametrosDaGrade().qtdeLinhas; 
		int qtdePontos = Parametros::getInstance()->getParametrosDaGrade().qtdePontos;
		this->grade = new Grade(9,-9,altura,qtdeLinha,qtdePontos);
	}
	return grade;
}

void PlanoDeCorte::setAltura(double h){
	this->altura = h;
	this->ator->setGlobalPosition(NxVec3(0,altura,0));
}
