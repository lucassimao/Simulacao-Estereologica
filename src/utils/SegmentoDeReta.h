#ifndef SEGMENTO_DE_RETA_H
#define SEGMENTO_DE_RETA_H

#include "Vetor.h"
#include "..\model\grade\RetaDeTeste.h"
#include "..\defs.h"
#include <stddef.h>

using namespace simulacao::model::grade;

class SegmentoDeReta{
public:
	explicit SegmentoDeReta(Ponto &p0,Ponto &p1);
	explicit SegmentoDeReta(Vetor &r0,Vetor &r1);

	bool interceptarComPlano(int yplano, Ponto *interceptacao);
	bool interceptar(RetaDeTeste&,Ponto*);

	// r= r0+ tvetorDiretor
	Vetor r0,r1,vetorDiretor;

};

#endif