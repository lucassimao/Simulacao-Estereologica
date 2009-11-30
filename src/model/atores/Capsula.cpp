#include "Capsula.h"

using namespace simulacao::model::atores;

Capsula::Capsula(NxScene *cena):Ator()
{
	this->raio_base = 0.5;
	this->altura = 2.0;

	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	NxCapsuleShapeDesc capsuleDesc;

	//sphereDesc.shapeFlags |= NX_SF_DYNAMIC_DYNAMIC_CCD;
	capsuleDesc.radius = this->raio_base;
	capsuleDesc.height = this->altura;

	//sphereDesc.ccdSkeleton = ccds;
	actorDesc.shapes.pushBack(&capsuleDesc);

	actorDesc.body = &bodyDesc;
	actorDesc.density=10.0f;
	float a = rand(); float px = 10*(a/32767 - 0.5);
	float b = rand(); float py = 10*(b/32767 - 0.5)+ 15.0;//Devido à altura do eixo y
	float c = rand(); float pz = 10*(c/32767 - 0.5);

	actorDesc.globalPose.t = NxVec3(px,py,pz);
	NxActor *capsula =  cena->createActor(actorDesc);
	capsula->userData = (void *)this;
	this->ator = capsula;
}

Capsula::~Capsula(void)
{
}

bool Capsula::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	return true;
}
