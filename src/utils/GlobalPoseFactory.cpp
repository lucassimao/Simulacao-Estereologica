#include "GlobalPoseFactory.h"
#include "..\model\Parametros.h"
#include <QDebug>

using namespace simulacao::utils;
using namespace simulacao::model;


GlobalPoseFactory* GlobalPoseFactory::instance = NULL;

GlobalPoseFactory::GlobalPoseFactory(){}


NxVec3 GlobalPoseFactory::newGlobalPosition(){
	int sinais[] = {-1,1};
	NxVec3 newGlobalPosition(0,0,0);
	double arestaDaCaixa = Parametros::getInstance()->getArestaDaCaixa();

	int sinal = sinais[rand() % 2];
	newGlobalPosition.x = sinal * fmod( rand() , (arestaDaCaixa/2) ); 
	sinal = sinais[rand() % 2];
	newGlobalPosition.y = Parametros::getInstance()->getCentroDeMassaDaCaixa() + sinal  * fmod( rand() , arestaDaCaixa/2); 
	sinal = sinais[rand() % 2];
	newGlobalPosition.z = sinal  * fmod(rand() , (arestaDaCaixa/2));  
	
	return newGlobalPosition;
}