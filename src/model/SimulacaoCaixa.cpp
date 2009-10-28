#include <QtDebug>
#include <NxPhysics.h>
#include "SimulacaoCaixa.h"
#include "../canvas/atores/Capsula.h"
#include "../canvas/atores/Cubo.h"
#include "../canvas/atores/Esfera.h"
#include "../canvas/atores/Ator.h"
#include "../draw/cooking.h"
#include "../draw/Stream.h"

using namespace simulacao::model;
using namespace simulacao::model::atores;
NxCCDSkeleton * ccds;

SimulacaoCaixa::SimulacaoCaixa(void)
{
	this->caixa = criarCaixa();
	this->atorPlanoDeCorte = 0;
	this->shapePlanoDeCorte = 0;
	this->exibirCaixa = true;
	this->exibirPontosTeste = false;
	this->exibirRetasTeste = false;
	this->exibirTampaCaixa =  true;

}

SimulacaoCaixa::~SimulacaoCaixa(void)
{

}

void SimulacaoCaixa::CreateMeshes(){
	NxVec3 dim(10,10,10);// vetor que determina o tamanho da caixa
	NxVec3 dim1(0.5f, 0.5f , 0.5f);

	NxU32 triangles[3 * 12] = { 
		0,1,3,
		0,3,2,
		3,7,6,
		3,6,2,
		1,5,7,
		1,7,3,
		4,6,7,
		4,7,5,
		1,0,4,
		5,1,4,
		4,0,2,
		4,2,6
	};
	NxVec3 points[8];


	//static mesh
	points[0].set( dim.x, -dim.y, -dim.z);
	points[1].set( dim.x, -dim.y,  dim.z);
	points[2].set( dim.x,  dim.y, -dim.z);
	points[3].set( dim.x,  dim.y,  dim.z);

	points[4].set(-dim.x, -dim.y, -dim.z);
	points[5].set(-dim.x, -dim.y,  dim.z);
	points[6].set(-dim.x,  dim.y, -dim.z);
	points[7].set(-dim.x,  dim.y,  dim.z);

	bool status = InitCooking();
	if (!status) {
		printf("\nError: Unable to initialize the cooking library. Please make sure that you have correctly installed the latest version of the AGEIA PhysX SDK.\n\n");
		exit(1);
	}

	NxTriangleMeshDesc		meshDesc;
	meshDesc.numVertices = 8;
	meshDesc.numTriangles = 12;
	meshDesc.pointStrideBytes = sizeof(NxVec3);
	meshDesc.triangleStrideBytes = 3 * sizeof(NxU32);
	meshDesc.points = points;
	meshDesc.triangles = triangles;
	MemoryWriteBuffer buf;
	status = CookTriangleMesh(meshDesc, buf);
	if (!status) {
		printf("Unable to cook a triangle mesh.");
		exit(1);
	}
	MemoryReadBuffer readBuffer(buf.data);
	staticTriangleMesh = physicsSDK->createTriangleMesh(readBuffer);

	dim = dim1;
	points[0].set( dim.x, -dim.y, -dim.z);
	points[1].set( dim.x, -dim.y,  dim.z);
	points[2].set( dim.x,  dim.y, -dim.z);
	points[3].set( dim.x,  dim.y,  dim.z);

	points[4].set(-dim.x, -dim.y, -dim.z);
	points[5].set(-dim.x, -dim.y,  dim.z);
	points[6].set(-dim.x,  dim.y, -dim.z);
	points[7].set(-dim.x,  dim.y,  dim.z);

	NxConvexMeshDesc convexDesc;
	convexDesc.numVertices = 8;
	convexDesc.numTriangles = 0;
	convexDesc.pointStrideBytes = sizeof(NxVec3);
	convexDesc.points = points;
	convexDesc.flags |= NX_CF_COMPUTE_CONVEX;

	buf.clear();
	status = CookConvexMesh(convexDesc, buf);
	if (!status) {
		printf("Unable to cook a convex mesh.");
		exit(1);
	}
	{
		MemoryReadBuffer readBuffer(buf.data);
		physicsSDK->createConvexMesh(readBuffer);
	}

	NxSimpleTriangleMesh stm;
	stm.numVertices = 8;
	stm.numTriangles = 12;
	stm.pointStrideBytes = sizeof(NxVec3);
	stm.triangleStrideBytes = sizeof(NxU32)*3;

	for (NxU32 i = 0; i < 8; i++)
		points[i].arrayMultiply(points[i], NxVec3(0.8f, 0.8f, 0.9f));

	stm.points = points;
	stm.triangles = triangles;
	stm.flags |= NX_MF_FLIPNORMALS;
	ccds = physicsSDK->createCCDSkeleton(stm);

	CloseCooking();
}

NxActor * SimulacaoCaixa::criarCaixa(){

	NxBodyDesc BodyDesc;
	CreateMeshes();
	NxActorDesc actorDesc;
	actorDesc.density = 100.0f;
	actorDesc.globalPose.t = NxVec3(0,15,0);// Define a altura dos objetos no eixo y
	NxTriangleMeshShapeDesc meshShapeDesc;

	meshShapeDesc.meshData = staticTriangleMesh;
	actorDesc.shapes.pushBack(&meshShapeDesc);
	return  cena->createActor(actorDesc);


}

void SimulacaoCaixa::adicionarObjeto(TipoDeGrao tipo,NxI64 qtde){
	switch(tipo){
		case ESFERA:
			for(long l=0;l<qtde;++l){
				new Esfera(cena,ccds);
			}
			break;
		case CAPSULA:
			for(long l=0;l<qtde;++l){
				new Capsula(cena);
			}
			break;
		case CUBO:
			for(long l=0;l<qtde;++l){
				new Cubo(cena,ccds);
			}
			break;
	}
}



void SimulacaoCaixa::novoPlanoDeCorte(){
	float altura = 6 + rand()%18;

	if (atorPlanoDeCorte == NULL){

		NxPlaneShapeDesc planeDesc;
		NxActorDesc actorDesc;
		actorDesc.globalPose.t = NxVec3(0,altura,0);
		planeDesc.normal = NxVec3(0.0f,1.0f,0.0f);
		planeDesc.d = 0;

		actorDesc.shapes.pushBack(&planeDesc);
		atorPlanoDeCorte = getCena()->createActor(actorDesc);

		//this->shapePlanoDeCorte = &(((NxPlaneShape *)atorPlanoDeCorte->getShapes()[0])->getPlane());
	}
	else
		atorPlanoDeCorte->setGlobalPosition(NxVec3(0,altura,0));
}

void SimulacaoCaixa::exibirPlanoDeCorte(){ 
	novoPlanoDeCorte(); 
}

void SimulacaoCaixa::esconderPlanoDeCorte(){ 
	if(atorPlanoDeCorte) {getCena()->releaseActor(*atorPlanoDeCorte);atorPlanoDeCorte=NULL;} 
}

void SimulacaoCaixa::selecionarGraosInterceptados(){
	NxU32 qtdeAtores = getCena()->getNbActors();
	NxActor** atores = getCena()->getActors();

	while (qtdeAtores--)
	{
		NxActor* ator = *atores++;

		{
			if (ator != caixa && ator!=atorPlanoDeCorte){
				Ator *a = (Ator *)ator->userData;
				NxVec3 pos = ator->getGlobalPosition();
				if (!a->estaInterceptadoPeloPlano(atorPlanoDeCorte->getGlobalPosition())){
					cena->releaseActor(*ator);
					a = NULL;
				}
			}

		}

	}
}

void SimulacaoCaixa::removerGraos(){
NxU32 qtdeAtores = getCena()->getNbActors();
	NxActor** atores = getCena()->getActors();

	while (qtdeAtores--)
	{
		NxActor* ator = *atores++;

		{
			if (ator != caixa && ator!=atorPlanoDeCorte){
				getCena()->releaseActor(*ator);
			}
		}

	}
}