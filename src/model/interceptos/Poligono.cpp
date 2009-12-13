#include "Poligono.h"
#include <vector>

using namespace simulacao::model::interceptos;
using std::vector;


Poligono::Poligono(Cor cor,vector<Ponto> vertices):Intercepto(cor){
	this->vertices = vertices;
}