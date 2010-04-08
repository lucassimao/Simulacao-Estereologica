#include <QtDebug>
#include <vector>
#include <NxPhysics.h>
#include <NxExportedUtils.h>
#include "SimulacaoCaixa.h"
#include "Parametros.h"
#include "atores\Esfera.h"
#include "atores\PrismaTriangular.h"
#include "atores\PrismaTriangularTruncado.h"
#include "atores\Ator.h"
#include "..\utils\GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h"
#include "..\draw\Stream.h"
#include "..\draw\cooking.h"


using std::vector;
using namespace simulacao::model;
using namespace simulacao::model::atores;
using namespace simulacao::model::interceptos;

NxCCDSkeleton * ccds;

SimulacaoCaixa::SimulacaoCaixa(void)
{
	this->meshFactory = new MeshFactory(this->physicsSDK);
	this->atorPlanoDeCorte = 0;
	this->shapePlanoDeCorte = 0;
	this->exibirCaixa = true;
	this->caixa = criarCaixa();
	this->exibirPontosTeste=true;
	this->exibirRetasTeste=true;
	
	criarCCDS();
}

SimulacaoCaixa::~SimulacaoCaixa(void)
{
	removerGraos();
}

void SimulacaoCaixa::criarCCDS(){
	MemoryWriteBuffer buf;
	NxVec3 dim(0.5f, 0.5f , 0.5f);

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


	points[0].set( dim.x, -dim.y, -dim.z);
	points[1].set( dim.x, -dim.y,  dim.z);
	points[2].set( dim.x,  dim.y, -dim.z);
	points[3].set( dim.x,  dim.y,  dim.z);

	points[4].set(-dim.x, -dim.y, -dim.z);
	points[5].set(-dim.x, -dim.y,  dim.z);
	points[6].set(-dim.x,  dim.y, -dim.z);
	points[7].set(-dim.x,  dim.y,  dim.z);

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

}

NxActor * SimulacaoCaixa::criarCaixa(){
	// definindo os vertices
	double arestaDaCaixadeGraos = Parametros::getInstance()->getArestaDaCaixa()/2.0;
	NxVec3 dim(arestaDaCaixadeGraos,arestaDaCaixadeGraos,arestaDaCaixadeGraos);

	NxU32 triangulos[3 * 12] = { 
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
	NxVec3 vertices[8];

	vertices[0].set( dim.x, -dim.y, -dim.z);
	vertices[1].set( dim.x, -dim.y,  dim.z);
	vertices[2].set( dim.x,  dim.y, -dim.z);
	vertices[3].set( dim.x,  dim.y,  dim.z);

	vertices[4].set(-dim.x, -dim.y, -dim.z);
	vertices[5].set(-dim.x, -dim.y,  dim.z);
	vertices[6].set(-dim.x,  dim.y, -dim.z);
	vertices[7].set(-dim.x,  dim.y,  dim.z);

	NxBodyDesc BodyDesc;
	NxActorDesc actorDesc;
	actorDesc.globalPose.t = NxVec3(0,15,0);// Define a altura dos objetos no eixo y
	NxTriangleMeshShapeDesc meshShapeDesc;

	meshShapeDesc.meshData = this->meshFactory->criarTriangleMesh(8,12,vertices,triangulos);
	actorDesc.shapes.pushBack(&meshShapeDesc);
	return cena->createActor(actorDesc);

}

void SimulacaoCaixa::adicionarObjeto(TipoDeGrao tipo,NxI64 qtde){
	switch(tipo){
		case ESFERA:
			for(long l=0;l<qtde;++l){
				new Esfera(cena,ccds);
			}
			break;
		case PRISMA_TRIANGULAR:
			for(long l=0;l<qtde;++l){
				new PrismaTriangular(cena,NULL,meshFactory);
			}
			break;
		case PRISMA_TRIANGULAR_TRUNCADO:
			for(long l=0;l<qtde;++l){
				new PrismaTriangularTruncado(cena,NULL,meshFactory);
			}
			break;
	}
}



void SimulacaoCaixa::novoPlanoDeCorte(){

	float altura = this->alturaPlanoStrategy->novaAltura();

	if (atorPlanoDeCorte == NULL){
		
		NxPlaneShapeDesc planeDesc;
		NxActorDesc actorDesc;
		actorDesc.globalPose.t = NxVec3(0,altura,0);
		planeDesc.normal = NxVec3(0.0f,1.0f,0.0f);
		planeDesc.d = 0;

		actorDesc.shapes.pushBack(&planeDesc);
		atorPlanoDeCorte = new PlanoDeCorte(getCena()->createActor(actorDesc));
		Parametros::getInstance()->addObserver(atorPlanoDeCorte);

	}
	else
		atorPlanoDeCorte->setAltura(altura); 
}

void SimulacaoCaixa::exibirPlanoDeCorte(){ 
	novoPlanoDeCorte();
}

void SimulacaoCaixa::esconderPlanoDeCorte(){ 
	if(atorPlanoDeCorte) {getCena()->releaseActor(*atorPlanoDeCorte->getNxActor());  delete atorPlanoDeCorte;atorPlanoDeCorte=NULL;} 
}

void SimulacaoCaixa::selecionarGraosInterceptados(){
	NxU32 qtdeAtores = getCena()->getNbActors();
	NxActor** atores = getCena()->getActors();

	while (qtdeAtores--)
	{
		NxActor* ator = *atores++;

		{
			if (ator != caixa && ator!=atorPlanoDeCorte->getNxActor()){
				Ator *a = (Ator *)ator->userData;
				if (!a->estaInterceptadoPeloPlano(atorPlanoDeCorte->getNxActor()->getGlobalPosition())){
					cena->releaseActor(*ator);
					delete a;
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
			if (ator != caixa && ator!=atorPlanoDeCorte->getNxActor()){
				getCena()->releaseActor(*ator);
			}
		}

	}
}