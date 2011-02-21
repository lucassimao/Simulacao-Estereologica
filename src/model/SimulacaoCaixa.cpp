#include <QtDebug>
#include <vector>
#include <QTimer>
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

SimulacaoCaixa::SimulacaoCaixa(void)
{
	this->meshFactory = new MeshFactory(this->physicsSDK);
	this->atorPlanoDeCorte = 0;
	this->exibirCaixa = true;
	this->_exibirPlanoDeCorte = true;
	criarCaixa();
	this->exibirPontosTeste=true;
	this->exibirRetasTeste=true;
	
}

SimulacaoCaixa::~SimulacaoCaixa(void)
{
	removerGraos();
}

void SimulacaoCaixa::criarCaixa(){

	if (this->caixa != NULL){
		this->cena->releaseActor(*this->caixa);
		this->caixa = NULL;
	}

	double arestaDaCaixaDeGraosRadii = Parametros::getInstance()->getArestaDaCaixa()/2.0;
	NxVec3 dim(arestaDaCaixaDeGraosRadii,arestaDaCaixaDeGraosRadii,arestaDaCaixaDeGraosRadii);
	
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

	NxTriangleMeshShapeDesc meshShapeDesc;
	meshShapeDesc.meshData = this->meshFactory->criarTriangleMesh(8,12,vertices,triangulos);

	NxActorDesc actorDesc;
	actorDesc.globalPose.t = NxVec3(0,arestaDaCaixaDeGraosRadii,0);
	actorDesc.shapes.pushBack(&meshShapeDesc);

	this->caixa = cena->createActor(actorDesc);

	if (this->atorPlanoDeCorte != NULL){
		NxActor *ator = atorPlanoDeCorte->getNxActor();
		this->cena->releaseActor(*ator);
		delete this->atorPlanoDeCorte;
		this->atorPlanoDeCorte= NULL;
		novoPlanoDeCorte();
	}

}

void SimulacaoCaixa::adicionarObjeto(TipoDeGrao tipo,NxI64 qtde,Cor cor){
	switch(tipo){
		case ESFERA:
			for(long l=0;l<qtde;++l)
				new Esfera(cena,cor);
			break;
		case PRISMA_TRIANGULAR:
			for(long l=0;l<qtde;++l){
				new PrismaTriangular(cena,meshFactory,cor);
			}
			break;
		case PRISMA_TRIANGULAR_TRUNCADO:
			for(long l=0;l<qtde;++l){
				new PrismaTriangularTruncado(cena,meshFactory,cor);
			}
			break;
	}
	
}


void SimulacaoCaixa::novoPlanoDeCorte(){

	float altura = this->alturaPlanoStrategy->novaAltura();

	if (atorPlanoDeCorte == NULL){		
		NxPlaneShapeDesc planeDesc;
		planeDesc.normal = NxVec3(0.0f,1.0f,0.0f);
		planeDesc.d = 0;

		NxActorDesc actorDesc;
		actorDesc.globalPose.t = NxVec3(0,altura,0);		
		actorDesc.shapes.pushBack(&planeDesc);
		atorPlanoDeCorte = new PlanoDeCorte(getCena()->createActor(actorDesc));
		Parametros::getInstance()->addObserver(atorPlanoDeCorte);
	}
	else
		atorPlanoDeCorte->setAltura(altura); 
}

void SimulacaoCaixa::exibirPlanoDeCorte(){ 
	this->_exibirPlanoDeCorte = true;
}

void SimulacaoCaixa::esconderPlanoDeCorte(){ 
	this->_exibirPlanoDeCorte = false;
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
		if (ator != caixa && ator!=atorPlanoDeCorte->getNxActor()){
			getCena()->releaseActor(*ator);
		}

	}
}

double SimulacaoCaixa::getVolumeFaseSolida(){
	double v = 0;
	NxU32 qtdeAtores = getCena()->getNbActors();
	NxActor** atores = getCena()->getActors();

	while (qtdeAtores--)
	{
		NxActor* ator = *atores++;
		if (ator != caixa && ator!=atorPlanoDeCorte->getNxActor()){
			Ator *a = (Ator *)ator->userData;
			v+= a->getVolume();
		}
	}
	return v;
}