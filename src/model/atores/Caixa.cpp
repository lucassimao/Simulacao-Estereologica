#include "Caixa.h"

using namespace simulacao::model::atores;

Caixa::Caixa(NxScene *cena):Ator()
{
}

Caixa::~Caixa(void)
{
}

bool Caixa::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	return true;
}

void Caixa::draw(bool useShapeUserData){
}