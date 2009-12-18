#include "SegmentoDeReta.h"
#include <QDebug>

SegmentoDeReta::SegmentoDeReta(Vetor &v0,Vetor &v1){
	this->r0 = v0;
	this->r1 = v1;
	this->vetorDiretor = v1-v0;
}

bool SegmentoDeReta::contemPonto(Ponto p){
	Vetor v(p);
	Vetor v1 = (v - this->r0);
	double t1 = v1.x/vetorDiretor.x;
	double t2 = v1.y/vetorDiretor.y;
	double t3 = v1.z/vetorDiretor.z;

	return (0<= t1 == t2 == t3 <= 1) ;
}

bool SegmentoDeReta::interceptarPlano(int xplano, int yplano,int zplano,Ponto *interceptacao ){
	return false;
}