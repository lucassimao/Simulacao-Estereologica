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
