#include "SegmentoDeReta.h"


SegmentoDeReta::SegmentoDeReta(Vetor &v0,Vetor &v1){
	this->r0 = v0;
	this->r1 = v1;
	this->vetorDiretor = v1-v0;
}

bool SegmentoDeReta::contemPonto(Ponto p){
	Vetor v(p);
	Vetor v1 = (v - this->r0);
	Vetor v2 = (this->r1 - this->r0);
	double t1 = v1.x/v2.x;
	double t2 = v1.y/v2.y;
	double t3 = v1.z/v2.z;

	return (0<= t1 == t2 == t3 <= 1) ;
}

bool SegmentoDeReta::interceptarPlano(int xplano, int yplano,int zplano,Ponto *interceptacao ){
	return false;
}
