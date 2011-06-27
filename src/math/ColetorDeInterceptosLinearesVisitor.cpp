#include <vector>
#include <assert.h>
#include <QDebug>
#include "ColetorDeInterceptosLinearesVisitor.h"

#include "..\model\interceptos\Disco.h"
#include "..\model\interceptos\Poligono.h"

using std::vector;
using namespace simulacao::math::mathVisitor;
using namespace simulacao::model::interceptos;

ColetorDeInterceptosLinearesVisitor::ColetorDeInterceptosLinearesVisitor(Grade *grade){
	this->grade = grade;
}

void ColetorDeInterceptosLinearesVisitor::visit(Disco *disco){
	double z0 = disco->centro.z + disco->raio;
	double z1 = disco->centro.z - disco->raio;

	vector<RetaDeTeste> linhas = this->grade->getLinhasNoIntervalo(z0,z1);
	vector<RetaDeTeste>::const_iterator iterator = linhas.begin();

	while(iterator!=linhas.end()){
			RetaDeTeste l = *iterator;
			
			double distanciaEntreALinhaEOCentroDoDisco = abs(disco->centro.z - l.linhaInicio.z);
			double comprimentoDoInterceptoLinear = 2*sqrt(pow(disco->raio,2)-pow(distanciaEntreALinhaEOCentroDoDisco,2));
			
			this->interceptosLineares.push_back(comprimentoDoInterceptoLinear);
			
			++iterator;
	}
	
}

void ColetorDeInterceptosLinearesVisitor::visit(Poligono *poligono){
	vector<InterceptoLinear*> interceptosLineares = poligono->getInterceptosLineares(this->grade);
	vector<InterceptoLinear*>::const_iterator iterator = interceptosLineares.begin();
	while(iterator != interceptosLineares.end()){
		InterceptoLinear* intercepto = *iterator;
		this->interceptosLineares.push_back(abs(intercepto->p0.x - intercepto->p1.x));
		iterator++;
	}
}