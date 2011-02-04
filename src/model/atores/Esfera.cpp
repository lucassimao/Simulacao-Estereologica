#include <stdexcept>
#include "Esfera.h"
#include "..\Parametros.h"
#include "..\..\model\interceptos\Intercepto.h"
#include "..\..\model\interceptos\Disco.h"
#include "..\..\utils\GlobalPoseFactory.h"

using std::runtime_error;
using namespace simulacao::model::atores;
using namespace simulacao::model::interceptos;
using namespace simulacao::utils;

Esfera::Esfera(NxScene *cena,Cor cor):Ator()
{
		this->raio = Parametros::getInstance()->getRaioEsfera();
		this->cor = cor;

		NxActorDesc actorDesc2;
		NxBodyDesc bodyDesc2;

		NxSphereShapeDesc sphereDesc;
		//sphereDesc.shapeFlags |= NX_SF_DYNAMIC_DYNAMIC_CCD;
		sphereDesc.radius = this->raio;

		//sphereDesc.ccdSkeleton = ccds;
		actorDesc2.shapes.pushBack(&sphereDesc);

		actorDesc2.body = &bodyDesc2;
		actorDesc2.density=10.0f;

		actorDesc2.globalPose.t = GlobalPoseFactory::getInstance()->newGlobalPosition();
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
		return new Disco(this->cor,p,raioDaRegiaoInterceptada,this->raio);
	}
	else
		throw new runtime_error("Esta esfera não está interceptada");
}

bool Esfera::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	NxVec3 pos = ator->getGlobalPosition();
	
	NxReal distanciaY = abs(planoGlobalPosition.y - pos.y);
	
	return (this->raio - distanciaY) >=0;
}

