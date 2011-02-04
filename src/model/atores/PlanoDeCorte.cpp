#include "PlanoDeCorte.h"

using namespace simulacao::model::atores;

PlanoDeCorte::PlanoDeCorte(NxActor * nxActor):Ator()
{	
	this->ator = nxActor;
	this->altura = nxActor->getGlobalPosition().y;
	this->grade = 0;
	Cor c = VERDE;
	this->cor = c;
}

PlanoDeCorte::~PlanoDeCorte(void){
}

void PlanoDeCorte::update(Observable* obs){
	this->grade = novaGrade(this->altura);
}

bool PlanoDeCorte::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	return false;
}

Grade* PlanoDeCorte::novaGrade(float h){
	int qtdeLinha = Parametros::getInstance()->getParametrosDaGrade().qtdeLinhas; 
	int qtdePontos = Parametros::getInstance()->getParametrosDaGrade().qtdePontos;
	float z0 = (Parametros::getInstance()->getArestaDaCaixa())/2.0f;
	float z1 = -1*z0;
	return new Grade(z0-0.5f, z1,h,qtdeLinha,qtdePontos);
}

Grade *PlanoDeCorte::getGrade(){
	if (!grade){
		this->grade = novaGrade(this->altura);
	}
	return grade;
}

void PlanoDeCorte::setAltura(double h){
	this->altura = h;
	this->ator->setGlobalPosition(NxVec3(0,altura,0));
	this->grade = novaGrade(h);
}
