#include "PlanoDeCorte.h"

using namespace simulacao::model::atores;

PlanoDeCorte::PlanoDeCorte(NxScene *cena):Ator()
{

}

PlanoDeCorte::~PlanoDeCorte(void)
{
}

bool PlanoDeCorte::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	return false;
}

