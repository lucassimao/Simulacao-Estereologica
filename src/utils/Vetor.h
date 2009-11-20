#ifndef VETOR_H
#define VETOR_H

/**
Representa um vetor no espaço 3D
com a origem em 0,0,0
*/
#include <string>
#include <sstream>
#include "Vetor.h"
#include "..\defs.h"

using std::string;
using std::ostringstream;

class Vetor {
public:
	Vetor();
	Vetor(double,double,double);
	Vetor(Ponto);
	double x;
	double y;
	double z;

	bool operator==(Vetor&);
	Vetor operator-(Vetor&);
	Vetor operator+(Vetor&);
	Vetor operator*(double);
	operator string();

};


#endif
