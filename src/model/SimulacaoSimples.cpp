#include <NxPhysics.h>
#include "SimulacaoSimples.h"


using namespace simulacao::model;

SimulacaoSimples::SimulacaoSimples(void)
{
	this->plano = criarPlano();
	this->caixa = criarCaixa();

}

SimulacaoSimples::~SimulacaoSimples(void)
{
}

NxActor * SimulacaoSimples::criarPlano(){
	NxPlaneShapeDesc desc;
	NxActorDesc actorDesc;
	
	actorDesc.shapes.pushBack(&desc);
	return cena->createActor(actorDesc);
}

NxActor * SimulacaoSimples::criarCaixa(){
	NxReal boxStartHeight = 10.5; 

	//ator
    NxActorDesc actorDesc;
    NxBodyDesc bodyDesc;

	// shape
    NxBoxShapeDesc boxDesc;
    boxDesc.dimensions.set(0.5,0.5,0.5);

    actorDesc.shapes.pushBack(&boxDesc);
    actorDesc.body = &bodyDesc;
    actorDesc.density = 10;
    actorDesc.globalPose.t = NxVec3(0,boxStartHeight,0);
    return cena->createActor(actorDesc);	

}