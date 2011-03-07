#include "Disco.h"
using namespace simulacao::model::interceptos;

#include "..\..\canvas\drawVisitor\AbstractDrawVisitor.h"
using namespace simulacao::canvas::drawVisitor;

#include "..\..\math\AbstractMathVisitor.h"
using namespace simulacao::math::mathVisitor;


#define _USE_MATH_DEFINES
#include <cmath>

Disco::Disco(Cor cor,Ponto c,double r, double raioDaEsferaDeOrigem):InterceptoDeArea(cor),raio(r),centro(c),raioDaEsferaDeOrigem(raioDaEsferaDeOrigem){
}

Disco::Disco(Ponto c,double r,double raioDaEsferaDeOrigem):InterceptoDeArea(),raioDaEsferaDeOrigem(raioDaEsferaDeOrigem),raio(r),centro(c){
}

void Disco::accept(AbstractDrawVisitor *visitor){
	visitor->visit(this);
}

vector<InterceptoLinear*> Disco::getInterceptosLineares(Grade *grade){
	vector<InterceptoLinear*> interceptosLineares;

	double z0 = this->centro.z + this->raio;
	double z1 = this->centro.z - this->raio;

	vector<RetaDeTeste> linhas = grade->getLinhasNoIntervalo(z0,z1);
	vector<RetaDeTeste>::const_iterator iterator = linhas.begin();

	while(iterator!=linhas.end()){
			RetaDeTeste l = *iterator;
			
			double delta = pow(2*this->centro.x,2.0) -4*( pow(this->centro.x,2.0) - pow(this->raio,2.0) + pow(l.linhaFim.z - this->centro.z,2.0));
			Ponto p0, p1;
			double x1 = (2*this->centro.x + sqrt(delta))/2.0;
			double x2 = (2*this->centro.x - sqrt(delta))/2.0;
			if (x1 < x2){
				p0.x = x1;
				p1.x = x2;
			}else{
				p0.x = x2;
				p1.x = x1;
			}
			p0.y = l.linhaInicio.y;
			p1.y = l.linhaInicio.y;
			p0.z = l.linhaInicio.z;
			p1.z = l.linhaInicio.z;

			InterceptoLinear *interceptoLinear = new InterceptoLinear(p0,p1);
			
			double distanciaEntreALinhaEOCentroDoDisco = abs(this->centro.z - l.linhaInicio.z);
			//double comprimentoDoInterceptoLinear = 2*sqrt(pow(this->raio,2)-pow(distanciaEntreALinhaEOCentroDoDisco,2));

			interceptosLineares.push_back(interceptoLinear);
			
			++iterator;
	}

	return interceptosLineares;
}

void Disco::accept(AbstractMathVisitor *visitor){
	visitor->visit(this);
}
double Disco::getArea(){
	return M_PI * pow(this->raio,2.0);
}

double Disco::getPerimetro(){
	return 2*M_PI*this->raio;
}