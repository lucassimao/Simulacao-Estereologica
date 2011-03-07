#include "InterceptoDeArea.h"
#include <stdexcept>

using std::runtime_error;
using namespace simulacao::model::interceptos;

InterceptoDeArea::InterceptoDeArea(){
	Cor vermelho = {1.0,0,0};
	this->cor = vermelho;
}

InterceptoDeArea::InterceptoDeArea(Cor cor){
	this->cor = cor;
}
