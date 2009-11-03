#include "Cone.h"

using namespace simulacao::model::atores;

Cone::Cone(NxScene *cena):Ator()
{
}

Cone::~Cone(void)
{
}

bool Cone::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	return true;
}

void Cone::draw(bool useShapeUserData){
}