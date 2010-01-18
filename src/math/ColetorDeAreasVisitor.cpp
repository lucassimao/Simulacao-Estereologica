#include <vector>
#include <assert.h>
#include "ColetorDeAreasVisitor.h"

#include "..\model\interceptos\Disco.h"
#include "..\model\interceptos\Poligono.h"

using std::vector;
using namespace simulacao::math::mathVisitor;
using namespace simulacao::model::interceptos;

ColetorDeAreasVisitor::ColetorDeAreasVisitor(Grade *grade){
	this->grade = grade;
}

void ColetorDeAreasVisitor::visit(Disco *disco){
	this->areas.push_back(disco->getArea());
}

void ColetorDeAreasVisitor::visit(Poligono *poligono){
	this->areas.push_back(poligono->getArea());
}