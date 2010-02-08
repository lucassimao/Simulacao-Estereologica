#include "PlanoDeCorte.h"

using namespace simulacao::model::atores;

PlanoDeCorte::PlanoDeCorte(NxActor * nxActor):Ator()
{	
	this->ator = nxActor;
	this->grade = 0;
}

PlanoDeCorte::~PlanoDeCorte(void)
{
}

bool PlanoDeCorte::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	return false;
}

