#include "GlobalPoseFactory.h"
#include "..\model\Parametros.h"


using namespace simulacao::utils;
using namespace simulacao::model;


GlobalPoseFactory* GlobalPoseFactory::instance = NULL;

GlobalPoseFactory::GlobalPoseFactory(){}


NxVec3 GlobalPoseFactory::newGlobalPosition(){
	NxVec3 newGlobalPosition(0,0,0);
	int arestaDaCaixa = (int) Parametros::getInstance()->getArestaDaCaixa();

	newGlobalPosition.x = arestaDaCaixa/4 - (rand() % (arestaDaCaixa/2)); 
	newGlobalPosition.y = 2 + rand() % (arestaDaCaixa-arestaDaCaixa/2); 
	newGlobalPosition.z = arestaDaCaixa/4 - (rand() % (arestaDaCaixa/2));  
	return newGlobalPosition;
}