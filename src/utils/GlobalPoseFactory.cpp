#include "GlobalPoseFactory.h"
#include "..\model\Parametros.h"


using namespace simulacao::utils;
using namespace simulacao::model;


GlobalPoseFactory* GlobalPoseFactory::instance = NULL;

GlobalPoseFactory::GlobalPoseFactory(){}


NxVec3 GlobalPoseFactory::newGlobalPosition(){
	NxVec3 newGlobalPosition(0,0,0);
	double arestaDaCaixa = Parametros::getInstance()->getArestaDaCaixa();

	newGlobalPosition.x = arestaDaCaixa/4 - fmod( rand() , (arestaDaCaixa/2) ); 
	newGlobalPosition.y = Parametros::getInstance()->getAlturaDaBaseDaCaixa() + fmod( rand() , (arestaDaCaixa - arestaDaCaixa/2) ); 
	newGlobalPosition.z = arestaDaCaixa/4 - fmod(rand() , (arestaDaCaixa/2));  
	return newGlobalPosition;
}