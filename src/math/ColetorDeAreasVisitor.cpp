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
	this->areaColetada = 0;
}

void ColetorDeAreasVisitor::visit(Disco *disco){
	double area = disco->getArea();
	this->areas.push_back(area);
	this->areaColetada += area;
}

void ColetorDeAreasVisitor::visit(Poligono *poligono){
	double area = poligono->getArea();
	this->areas.push_back(area);
	this->areaColetada += area;
}