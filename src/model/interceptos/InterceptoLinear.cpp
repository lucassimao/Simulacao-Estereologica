#include "InterceptoLinear.h"

using namespace simulacao::model::interceptos;

InterceptoLinear::InterceptoLinear(Ponto pInicial,Ponto pFinal):p0(pInicial),p1(pFinal){
}

double InterceptoLinear::tamanho(){
	return pow(pow(p0.x - p1.x,2) + pow(p0.y - p1.y,2) + pow(p0.z - p1.z,2),0.5); 
}
