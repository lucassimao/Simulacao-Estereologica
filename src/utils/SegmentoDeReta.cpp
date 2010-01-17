#include "SegmentoDeReta.h"
#include <QDebug>

SegmentoDeReta::SegmentoDeReta(Ponto &p0,Ponto &p1){
	this->r0 = Vetor(p0.x,p0.y,p0.z);
	this->r1 = Vetor(p1.x,p1.y,p1.z);
	this->vetorDiretor = this->r1- this->r0;
}

SegmentoDeReta::SegmentoDeReta(Vetor &r0,Vetor &r1){
	this->r0 = r0;
	this->r1 = r1;
	this->vetorDiretor = r1-r0;
}

bool SegmentoDeReta::interceptar(SegmentoDeReta &sr,double *t,double *s){
	double det;

	det = (sr.r1.x - sr.r0.x) * (this->r1.y - this->r0.y)  -  (sr.r1.y - sr.r0.y) * (this->r1.x - this->r0.x);

	 if (det == 0.0)
	  return false ;

	 if (s!=NULL)
		*s = ((sr.r1.x - sr.r0.x) * (sr.r0.y - this->r0.y) - (sr.r1.y - sr.r0.y) * (sr.r0.x - this->r0.x))/ det ;
	 
	 if (t != NULL)
		*t =  ((this->r1.x - this->r0.x) * (sr.r0.y - this->r0.y) - (this->r1.y - this->r0.y) * (sr.r0.x - this->r0.x))/ det ;

	 return true;
}



bool SegmentoDeReta::interceptarComPlano(int yplano,Ponto *interceptacao){
	// y = this->r0.y +t*this->vetorDiretor.y;
	double t = (yplano - this->r0.y)/this->vetorDiretor.y;
	
	if (t>=0 && t<=1){
		interceptacao->y = yplano;
		interceptacao->x = this->r0.x + t*this->vetorDiretor.x;
		interceptacao->z = this->r0.z + t*this->vetorDiretor.z;
		return true;
	}
	else
		return false;
}