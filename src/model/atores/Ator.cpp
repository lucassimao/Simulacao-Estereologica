#include "Ator.h"
#include <GL/glut.h>
#include <stdexcept>
#include <NxPhysics.h>
#include "../../draw/DrawObjects.h"
#include "../../draw/UserData.h"

using namespace simulacao::model::atores;
using std::runtime_error;

Ator::Ator()
{
	this->cor.r =1;
	this->cor.g =1;
	this->cor.b = 1;

}

Ator::~Ator(void)
{
}

bool Ator::estaForaDaCaixa(NxVec3 globalPositionDaCaixa,double arestaDaCaixa){
	NxVec3 globalPositionAtor = this->ator->getGlobalPosition();
	double metadeArestaDaCaixa = arestaDaCaixa/2.0 + 1;

	if (globalPositionAtor.x <=  globalPositionDaCaixa.x - metadeArestaDaCaixa) return true;
	if (globalPositionAtor.x >=  globalPositionDaCaixa.x + metadeArestaDaCaixa) return true;

	if (globalPositionAtor.y <=  globalPositionDaCaixa.y - metadeArestaDaCaixa) return true;
	if (globalPositionAtor.y >=  globalPositionDaCaixa.y + metadeArestaDaCaixa) return true;

	if (globalPositionAtor.z <=  globalPositionDaCaixa.z - metadeArestaDaCaixa) return true;
	if (globalPositionAtor.z >=  globalPositionDaCaixa.z + metadeArestaDaCaixa) return true;
	return false;
}