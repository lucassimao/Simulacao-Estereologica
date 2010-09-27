#ifndef JARVIS_MARCH_H
#define JARVIS_MARCH_H

#include <list>
using std::list;

#include <map>
using std::map;

#include "..\defs.h"

typedef enum {
	Negativo,Positivo
} EixoX;

#ifdef TEST_ENV
	inline double radianos2Graus(double radianos);
	inline double calcularAlguloRelativoAoEixoX(Ponto pInicio,Ponto pFim,EixoX eixoX);
	inline Ponto getVerticeComMaiorZ(list<Ponto> *vertices);
	inline Ponto getVerticeComMenorZ(list<Ponto> *vertices);
	inline Ponto getVerticeMaisDistante(Ponto pInicio,Ponto p1,Ponto p2);
#endif
list<Ponto> ordenarVertices(list<Ponto> vertices);

#endif