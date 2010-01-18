#include "Disco.h"
using namespace simulacao::model::interceptos;

#include "..\..\canvas\drawVisitor\AbstractDrawVisitor.h"
using namespace simulacao::canvas::drawVisitor;

#define _USE_MATH_DEFINES
#include <cmath>

Disco::Disco(Cor cor,Ponto c,double r):Intercepto(cor),raio(r),centro(c){
}

Disco::Disco(Ponto c,double r):Intercepto(),raio(r),centro(c){
}

void Disco::accept(AbstractDrawVisitor *visitor){
	visitor->visit(this);
}

double Disco::calcularArea(){
	return M_PI * pow(this->raio,2.0);
}
