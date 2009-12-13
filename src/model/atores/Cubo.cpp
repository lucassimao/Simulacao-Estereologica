#include "Cubo.h"
#include <NxExportedUtils.h>
#include "..\..\model\interceptos\Poligono.h"

using namespace simulacao::model::interceptos;
using namespace simulacao::model::atores;

Cubo::Cubo(NxScene *cena,double aresta):Ator()
{
	this->lado = aresta;

	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	NxBoxShapeDesc boxDesc;

	//sphereDesc.shapeFlags |= NX_SF_DYNAMIC_DYNAMIC_CCD;
	boxDesc.dimensions.set(this->lado,this->lado,this->lado);

	//boxDesc.ccdSkeleton = ccds;
	actorDesc.shapes.pushBack(&boxDesc);

	actorDesc.body = &bodyDesc;
	actorDesc.density=10.0f;
	float a = rand(); float px = 10*(a/32767 - 0.5);
	float b = rand(); float py = 10*(b/32767 - 0.5)+ 15.0;//Devido à altura do eixo y
	float c = rand(); float pz = 10*(c/32767 - 0.5);

	actorDesc.globalPose.t = NxVec3(px,py,pz);
	NxActor *box =  cena->createActor(actorDesc);
	box->userData = (void *)this;
	this->ator = box;
}

Intercepto* Cubo::getIntercepto(NxVec3 planoGlobalPosition){
	Cor v = VERMELHO;
	vector<Ponto> vertices;
	return NULL;
}

Cubo::~Cubo(void)
{
}

bool Cubo::estaInterceptadoPeloPlano(NxVec3 planoGlobalPosition){
	NxVec3 vertices[8];
	NxVec3 pos = ator->getGlobalPosition();
	NxBox box;
	this->ator->getShapes()[0]->isBox()->getWorldOBB(box);
	// método que retorna os vértices da caixa
	NxComputeBoxPoints(box,vertices);



	for(int i=0;i<8;++i){
		//distância da altura do centro do cubo para a altura do plano
		NxReal distancia_Y2 = abs(pos.y - planoGlobalPosition.y);
		//distância da altura do centro do cubo para o vértice
		NxReal distancia_Y3 = abs(pos.y - vertices[i].y);
		if (distancia_Y2 <= distancia_Y3)
			return true;
	}
	return false;
}