#include "Poligono.h"
#include <vector>

using namespace simulacao::canvas::interceptos;
using std::vector;


Poligono::Poligono(Cor cor,vector<Ponto> vertices):Intercepto(cor){
	this->vertices = vertices;
}

DrawCommand* Poligono::getDrawCommand(){
	return NULL;
}
