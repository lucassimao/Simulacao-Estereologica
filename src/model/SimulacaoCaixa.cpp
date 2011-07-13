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
#include "..\draw\UpdateTime.h"

using std::vector;
using namespace simulacao::model;
using namespace simulacao::model::atores;
using namespace simulacao::model::interceptos;

SimulacaoCaixa::SimulacaoCaixa(double coeficienteDeInterpenetracao,double aceleracaoGravidade):gravidade(NxVec3(0,aceleracaoGravidade,0)),fatorDeInterpenetracao(coeficienteDeInterpenetracao)
{
	this->physicsSDK = 0;
	this->deltaTime = 1.0/60.0;
	this->cena = 0;
	this->simulacaoEmHardware = true;
	this->status = PAUSADO;

	initPhysicsSDK();

	this->meshFactory = new MeshFactory(this->physicsSDK);
	this->atorPlanoDeCorte = 0;
	this->exibirCaixa = true;
	this->_exibirPlanoDeCorte = true;
	criarCaixa();
	this->exibirPontosTeste=true;
	this->exibirRetasTeste=true;

	sqlite3 *db = DataBaseFactory::getInstance()->criarBanco(":memory:");
	
	if (!db){
		qDebug() << sqlite3_errmsg(db);
		sqlite3_close(db);
		throw runtime_error( sqlite3_errmsg(db) );	
	}

	this->dao = new DAO(db);
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

void SimulacaoCaixa::adicionarEsferas(int qtde,Cor cor){
	for(int l=0;l<qtde;++l)
		new Esfera(cena,cor);
}

void SimulacaoCaixa::adicionarPrismas(double L0, int quantidade, Cor cor, double razaoDeAspecto, double razaoDeTruncamento){
	if (razaoDeTruncamento == 0){
		for(int l=0;l<quantidade;++l){
			new PrismaTriangular(cena,meshFactory,cor);
			this->dao->salvarPrisma(razaoDeAspecto,razaoDeTruncamento,L0);
		}
	}else{
		for(int l=0;l<quantidade;++l){
			new PrismaTriangularTruncado(cena,meshFactory,cor);
			this->dao->salvarPrisma(razaoDeAspecto,razaoDeTruncamento,L0);
		}	
	}
}

sqlite3 * SimulacaoCaixa::executarCortesSistematicos(int qtdeDeCortesSistematicos){

	this->dao->zerar();
	Parametros *p = Parametros::getInstance();
	double h0 = p->getAlturaDaBaseDaCaixa();
	double h1 = p->getArestaDaCaixa() + p->getAlturaDaBaseDaCaixa();
	this->setGeradorDeAlturaDoPlanoStrategy(new GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy(h0,h1,qtdeDeCortesSistematicos));

	int qtdeDePontosNaGrade = p->getQtdeLinhasNaGrade()* p->getQtdePontosPorLinhaNaGrade();
	double areaDoPlanoDeCorte = pow(p->getLarguraDoPlanoDeCorte(),2.0);
	double volumeFaseSolida = this->getVolumeFaseSolida();
	double volumeFaseLigante = this->getVolumeFaseLigante();

	for(int i=0; i< qtdeDeCortesSistematicos; ++i){
		this->novoPlanoDeCorte();

		NxActor** atores = this->getAtores();
		NxVec3 planoGlobalPosition = this->atorPlanoDeCorte->getNxActor()->getGlobalPosition();
		NxU32 qtdeAtores = this->getQtdeObjetos();

		ColetorDePontosVisitor *visitor2 = new ColetorDePontosVisitor(this->getGrade());
		ColetorDeAreasVisitor *visitor3 = new ColetorDeAreasVisitor(this->getGrade());

		__int64 planoID =  this->dao->salvarPlano(planoGlobalPosition.y,p->getLarguraDoPlanoDeCorte(),this->getPlanoDeCorte()->cor);

		qDebug() << " Plano " << planoID<< endl;

		while (qtdeAtores--)
		{
			NxActor* ator = *atores++;
			if (ator != this->caixa && ator!= this->atorPlanoDeCorte->getNxActor()){
				Ator *a = (Ator *)ator->userData;	

				if (a->estaInterceptadoPeloPlano(planoGlobalPosition)){
					InterceptoDeArea *intercepto = a->getIntercepto(planoGlobalPosition);

					intercepto->accept(visitor2);
					intercepto->accept(visitor3);

					__int64 interceptoID =  this->dao->salvarInterceptoDeArea(planoID,intercepto);

					vector<InterceptoLinear*> interceptosLineares = intercepto->getInterceptosLineares(this->getGrade());	
					/*for(int i=0;i< interceptosLineares.size();++i){
						Ponto p0 =interceptosLineares[i]->p0;
						Ponto p1 =interceptosLineares[i]->p1;

						qDebug() << p0.x << "," << p0.y<< ","<<p0.z << " ;;; " << p1.x << "," << p1.y<< ","<<p1.z<<endl;
					}*/
					this->dao->salvarInterceptosLineares(interceptoID,interceptosLineares,intercepto->getType());

				}
			}
		}
		this->dao->salvarInterceptosPorosos(planoID);

		double areaTotalColetada = visitor3->getAreaTotalColetada();
		int qtdePontosInternosAInterceptosDeArea = visitor2->getQtdeDePontosInternosAInterceptosDeArea();

		this->dao->salvarEstatisticas(planoID,areaTotalColetada,areaDoPlanoDeCorte,qtdePontosInternosAInterceptosDeArea,
			qtdeDePontosNaGrade,volumeFaseSolida,volumeFaseLigante);
	}
	this->setGeradorDeAlturaDoPlanoStrategy(new GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy());

	return this->dao->getDB();
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
		if (ator != caixa && ator!=atorPlanoDeCorte->getNxActor()){
			Ator *a = (Ator *)ator->userData;
			if (!a->estaInterceptadoPeloPlano(atorPlanoDeCorte->getNxActor()->getGlobalPosition())){
				cena->releaseActor(*ator);
				delete a;
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

	sqlite3 *db = DataBaseFactory::getInstance()->criarBanco(":memory:");
	
	if (!db){
		qDebug() << sqlite3_errmsg(db);
		sqlite3_close(db);
		throw runtime_error( sqlite3_errmsg(db) );	
	}

	this->dao = new DAO(db);
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

SimulacaoCaixa::~SimulacaoCaixa(void)
{
    pararSimulacao();
	removerGraos();
}

void SimulacaoCaixa::pararSimulacao(){
    if (cena)
	{
		GetPhysicsResults(); 
		physicsSDK->releaseScene(*cena);
	}
	if (physicsSDK)  physicsSDK->release();
	this->status = PARADO;
}

void SimulacaoCaixa::initPhysicsSDK() throw (exception){

	physicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);

	if (!physicsSDK){
		throw new exception("O SDK PhysX não foi inicializado com sucesso!");
	}

	physicsSDK->setParameter(NX_SKIN_WIDTH, this->fatorDeInterpenetracao);
	physicsSDK->setParameter(NX_CONTINUOUS_CD, 1);
	physicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	physicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	physicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);
	physicsSDK->setParameter(NX_CCD_EPSILON, 0.01);

	NxSceneDesc sceneDesc;
	sceneDesc.gravity = gravidade;
	sceneDesc.simType = NX_SIMULATION_HW;
	cena = physicsSDK->createScene(sceneDesc);

	if (!cena){
		sceneDesc.simType = NX_SIMULATION_SW;
		cena = physicsSDK->createScene(sceneDesc);
		if (!cena) exit(-1);
		this->simulacaoEmHardware = false;

	}

	NxMaterial* defaultMaterial = cena->getMaterialFromIndex(0); 
    defaultMaterial->setRestitution(0.5);
    defaultMaterial->setStaticFriction(0.5);
    defaultMaterial->setDynamicFriction(0.5);
		
	UpdateTime();

}


void SimulacaoCaixa::iniciarSimulacao(){
	this->status = EXECUCAO;
	deltaTime = UpdateTime();

    cena->simulate(deltaTime);
    cena->flushStream();

}

void SimulacaoCaixa::GetPhysicsResults()
{
    while (!cena->fetchResults(NX_RIGID_BODY_FINISHED, false));
}


NxI64 SimulacaoCaixa::getQtdeObjetos() const{
	return cena->getNbActors();
}