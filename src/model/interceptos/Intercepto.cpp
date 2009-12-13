#include "Intercepto.h"

using namespace simulacao::model::interceptos;

Intercepto::Intercepto(){
	Cor vermelho = {1.0,0,0};
	this->cor = vermelho;
}

Intercepto::Intercepto(Cor cor){
	this->cor = cor;
}
