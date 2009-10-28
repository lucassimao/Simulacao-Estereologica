#include <NxPhysics.h>
#include <stdexcept>
#include "../model/Simulacao.h"
#include "../draw/UpdateTime.h"

using namespace simulacao::model;
using std::exception;

const NxVec3 Simulacao::gravidadeDefault(0,-9.8,0);

Simulacao::Simulacao(void)
{
	this->qtdeObjetos = 0;
	this->gravidade = new NxVec3(0,0,0);
	this->pontoDeVisualizacao = new NxVec3(0,0,0);
	this->zoom = 10;
	this->physicsSDK = 0;
	this->deltaTime = 1.0/60.0;
	this->cena = 0;
	this->simulacaoEmHardware = true;
	this->status = PAUSADO;

	initPhysicsSDK();

}

Simulacao::~Simulacao(void)
{
    if (cena)
	{
		GetPhysicsResults();  
		physicsSDK->releaseScene(*cena);
	}
	if (physicsSDK)  physicsSDK->release();

}
void Simulacao::initPhysicsSDK() throw (exception){

	physicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);

	if (!physicsSDK){
		throw new exception("O SDK PhysX não foi inicializado com sucesso!");
	}

	physicsSDK->setParameter(NX_SKIN_WIDTH, 0.01);
	physicsSDK->setParameter(NX_CONTINUOUS_CD, 1);
	physicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	physicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	physicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);
	physicsSDK->setParameter(NX_CCD_EPSILON, 0.01);

	NxSceneDesc sceneDesc;
	sceneDesc.gravity = *gravidade;
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


void Simulacao::iniciarSimulacao(){
	this->status = EXECUCAO;
	deltaTime = UpdateTime();

    cena->simulate(deltaTime);
    cena->flushStream();

}

void Simulacao::GetPhysicsResults()
{
    while (!cena->fetchResults(NX_RIGID_BODY_FINISHED, false));
}




