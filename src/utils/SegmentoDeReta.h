#ifndef SEGMENTO_DE_RETA_H
#define SEGMENTO_DE_RETA_H

#include "Vetor.h"
#include "..\defs.h"
#include <stddef.h>

class SegmentoDeReta{
public:
	explicit SegmentoDeReta(Vetor &v0,Vetor &v1) ;
	bool contemPonto(Ponto p);
	bool interceptarPlano(int xplano, int yplano,int zplano,Ponto *interceptacao);
private:
	// r= r0+ tvetorDiretor
	Vetor r0,vetorDiretor,r1;

};

#endif