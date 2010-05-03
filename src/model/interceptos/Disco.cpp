#include "Disco.h"
using namespace simulacao::model::interceptos;

#include "..\..\canvas\drawVisitor\AbstractDrawVisitor.h"
using namespace simulacao::canvas::drawVisitor;

#include "..\..\math\AbstractMathVisitor.h"
using namespace simulacao::math::mathVisitor;


#define _USE_MATH_DEFINES
#include <cmath>

Disco::Disco(Cor cor,Ponto c,double r, double raioDaEsferaDeOrigem):Intercepto(cor),raio(r),centro(c),raioDaEsferaDeOrigem(raioDaEsferaDeOrigem){
}

Disco::Disco(Ponto c,double r,double raioDaEsferaDeOrigem):Intercepto(),raioDaEsferaDeOrigem(raioDaEsferaDeOrigem),raio(r),centro(c){
}

void Disco::accept(AbstractDrawVisitor *visitor){
	visitor->visit(this);
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