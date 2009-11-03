#include "Esfera.h"

using namespace simulacao::model::atores;

Esfera::Esfera(NxScene *cena,NxCCDSkeleton *ccds):Ator()
{
		this->raio = 0.9;

		NxActorDesc actorDesc2;
		NxBodyDesc bodyDesc2;

		NxSphereShapeDesc sphereDesc;
		//sphereDesc.shapeFlags |= NX_SF_DYNAMIC_DYNAMIC_CCD;
		sphereDesc.radius = this->raio;

		sphereDesc.ccdSkeleton = ccds;
		actorDesc2.shapes.pushBack(&sphereDesc);

		actorDesc2.body = &bodyDesc2;
		actorDesc2.density=10.0f;
		float a = rand(); float px = 10*(a/32767 - 0.5);
		float b = rand(); float py = 10*(b/32767 - 0.5)+ 15.0;//Devido à altura do eixo y
		float c = rand(); float pz = 10*(c/32767 - 0.5);

		actorDesc2.globalPose.t = NxVec3(px,py,pz);
		NxActor *esfera =  cena->createActor(actorDesc2);
		esfera->userData = (void *)this;
		this->ator = esfera;
}

Esfera::~Esfera(void)
{
}

bool Esfera::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	NxVec3 pos = ator->getGlobalPosition();
	
	NxReal distanciaY = abs(planoGlobalPosition.y - pos.y);
	
	return (this->raio - distanciaY) >=0;
}

void Esfera::draw(bool useShapeUserData){

}

