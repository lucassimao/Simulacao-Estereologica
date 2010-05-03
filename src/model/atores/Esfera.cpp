#include <stdexcept>
#include "Esfera.h"
#include "..\Parametros.h"
#include "..\..\model\interceptos\Intercepto.h"
#include "..\..\model\interceptos\Disco.h"

using std::runtime_error;
using namespace simulacao::model::atores;
using namespace simulacao::model::interceptos;

Esfera::Esfera(NxScene *cena,NxCCDSkeleton *ccds):Ator()
{
		this->raio = Parametros::getInstance()->getRaioEsfera();
		this->cor = Parametros::getInstance()->getCorEsfera();

		NxActorDesc actorDesc2;
		NxBodyDesc bodyDesc2;

		NxSphereShapeDesc sphereDesc;
		//sphereDesc.shapeFlags |= NX_SF_DYNAMIC_DYNAMIC_CCD;
		sphereDesc.radius = this->raio;

		sphereDesc.ccdSkeleton = ccds;
		actorDesc2.shapes.pushBack(&sphereDesc);

		actorDesc2.body = &bodyDesc2;
		actorDesc2.density=10.0f;

		float px = 9 - (rand() % 20);
		float py = 5 + (rand() % 20);
		float pz = 9 - (rand() % 20);

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
		return new Disco(p,raioDaRegiaoInterceptada,this->raio);
	}
	else
		throw new runtime_error("Esta esfera não está interceptada");
}

bool Esfera::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	NxVec3 pos = ator->getGlobalPosition();
	
	NxReal distanciaY = abs(planoGlobalPosition.y - pos.y);
	
	return (this->raio - distanciaY) >=0;
}

