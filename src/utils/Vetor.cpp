#include "Vetor.h"


Vetor::Vetor(){
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vetor::Vetor(double x,double y,double z){
	this->x = x;
	this->y = y;
	this->z = z;
}
Vetor::Vetor(Ponto p){
	this->x = p.x;
	this->y = p.y;
	this->z = p.z;
}
bool Vetor::operator==(const Vetor& other) const{
	return (this->x == other.x && this->y == other.y && this->z == other.z);
}
bool Vetor::operator!=(const Vetor& other) const{
	return (this->x != other.x || this->y != other.y || this->z != other.z);
}
Vetor Vetor::operator-(Vetor& other){
	return Vetor(this->x - other.x, this->y - other.y, this->z -other.z);
}
Vetor Vetor::operator+(Vetor& other){
	return Vetor(this->x + other.x, this->y + other.y, this->z + other.z);
}
Vetor Vetor::operator*(double fator){
	return Vetor(this->x*fator, this->y *fator, this->z *fator);
}

Vetor::operator string(){
	ostringstream ostream;
	ostream << "(" << this->x  <<  "," << this->y << "," << this->z << ")";
	return ostream.str();
}