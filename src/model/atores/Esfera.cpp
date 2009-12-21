#include <stdexcept>
#include "Esfera.h"
#include "..\..\model\interceptos\Intercepto.h"
#include "..\..\model\interceptos\Disco.h"

using std::runtime_error;
using namespace simulacao::model::atores;
using namespace simulacao::model::interceptos;

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
		float a = rand(); float px = 12*(a/32767 - 0.5);
		float b = rand(); float py = 12*(b/32767 - 0.5)+ 15.0;//Devido à altura do eixo y
		float c = rand(); float pz = 12*(c/32767 - 0.5);

		actorDesc2.globalPose.t = NxVec3(px,py,pz);
		NxActor *esfera =  cena->createActor(actorDesc2);
		esfera->userData = (void *)this;
		this->ator = esfera;
}

Esfera::~Esfera(void)
{
}
Intercepto* Esfera::getIntercepto(NxVec3 planoGlobalPosition){
	if (estaInterceptadoPeloPlano(planoGlobalPosition))
	{
		NxVec3 pos = ator->getGlobalPosition();
		NxReal distanciaDoPlanoAoCentroDaEsfera = abs(planoGlobalPosition.y - pos.y);
		
		NxReal raioDaRegiaoInterceptada= sqrt(pow(this->raio,2) - pow(distanciaDoPlanoAoCentroDaEsfera,2));
		
		Ponto p  = {pos.x,planoGlobalPosition.y,pos.z};
		return new Disco(p,raioDaRegiaoInterceptada);
	}
	else
		throw new runtime_error("Esta esfera não está interceptada");
}

bool Esfera::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	NxVec3 pos = ator->getGlobalPosition();
	
	NxReal distanciaY = abs(planoGlobalPosition.y - pos.y);
	
	return (this->raio - distanciaY) >=0;
}

