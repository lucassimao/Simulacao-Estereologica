#include "SegmentoDeReta.h"
#include <QDebug>

SegmentoDeReta::SegmentoDeReta(Vetor &r0,Vetor &r1){
	this->r0 = r0;
	this->r1 = r1;
	this->vetorDiretor = r1-r0;
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