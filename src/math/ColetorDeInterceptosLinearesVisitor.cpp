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
	double z0 = poligono->verticeComMaiorZ.z;
	double z1 = poligono->verticeComMenorZ.z;
	
	vector<RetaDeTeste> linhas = this->grade->getLinhasNoIntervalo(z0,z1);
	vector<RetaDeTeste>::const_iterator iterator = linhas.begin();
	
	while(iterator!=linhas.end()){
			RetaDeTeste retaDeTeste = *iterator;

			list<SegmentoDeReta> arestasInterceptadas = poligono->getArestasInterceptadas(retaDeTeste);
			if  (arestasInterceptadas.size()==2){
				SegmentoDeReta seg1 = arestasInterceptadas.front();
				Ponto p1;
				seg1.interceptar(retaDeTeste,&p1);
				
				SegmentoDeReta seg2 = arestasInterceptadas.back();
				Ponto p2;
				seg2.interceptar(retaDeTeste,&p2);

				double distancia = pow(pow(p1.x - p2.x,2) + pow(p1.y - p2.y,2) + pow(p1.z - p2.z,2),0.5); 
				this->interceptosLineares.push_back(distancia);
			}			

			++iterator;
		}
}