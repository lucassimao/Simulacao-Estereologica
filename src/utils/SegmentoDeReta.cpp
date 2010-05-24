#include <QtDebug>
#include "SegmentoDeReta.h"
#include "..\model\grade\RetaDeTeste.h"

using namespace simulacao::model::grade;

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

bool SegmentoDeReta::interceptar(RetaDeTeste& retaDeTeste,Ponto *pontoInterseccao){
	SegmentoDeReta segmentoDeTeste(retaDeTeste.linhaInicio,retaDeTeste.linhaFim);
	
	double s = (retaDeTeste.linhaInicio.z - this->r0.z)/(this->r1.z - this->r0.z);
	if (s < 0 || s > 1)
		return false;

	if (pontoInterseccao){
		double t = (this->r0.x + s*( this->r1.x - this->r0.x) - segmentoDeTeste.r0.x)/(segmentoDeTeste.r1.x - segmentoDeTeste.r0.x);
		pontoInterseccao->x = (segmentoDeTeste.r0 + segmentoDeTeste.vetorDiretor*t).x;
		pontoInterseccao->y = segmentoDeTeste.r0.y;
		pontoInterseccao->z = segmentoDeTeste.r0.z;

	}
	return true;

}

bool SegmentoDeReta::interceptarComPlano(double yplano,Ponto *interceptacao){
	// y = this->r0.y +t*this->vetorDiretor.y;
	double t = (yplano - this->r0.y)/this->vetorDiretor.y;
	qDebug() << t << endl;
	if (t>=0 && t<=1){
		interceptacao->y = yplano;
		interceptacao->x = this->r0.x + t*this->vetorDiretor.x;
		interceptacao->z = this->r0.z + t*this->vetorDiretor.z;
		return true;
	}
	else
		return false;
}