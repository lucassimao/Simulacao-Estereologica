#include   <cstdio>
#include   <cmath>
#include   <cstdlib>
#include <cassert>

#include <list>
using std::list;

#include <map>
using std::map;


#include "jarvis.h"

#define PI 3.1415926535

inline double radianos2Graus(double radianos){
	return radianos*(180.0/PI);
}

inline double calcularAlguloRelativoAoEixoX(Ponto pInicio,Ponto pFim,EixoX eixo){
	double seno = abs(pFim.z - pInicio.z)/sqrt(pow(pFim.x - pInicio.x,2.0) + pow(pFim.z - pInicio.z,2.0));
	double anguloEmRadianos = asin(seno);
	double anguloEmGraus = radianos2Graus(anguloEmRadianos);
	
	switch(eixo){
		case Positivo:
			if ( pInicio.z < pFim.z){
				if (pInicio.x > pFim.x)
					anguloEmGraus += 180;
				else 
					anguloEmGraus = 360  - anguloEmGraus;
			}else{
				if (pInicio.x > pFim.x)
					anguloEmGraus = 180  - anguloEmGraus;			
			}
			break;
		case Negativo:
			if ( pInicio.z < pFim.z ){
				if (pInicio.x < pFim.x)
					anguloEmGraus = 180 - anguloEmGraus;
			}else{
				if (pInicio.x > pFim.x)
					anguloEmGraus = 360  - anguloEmGraus;
				else 
					anguloEmGraus += 180;		
			}
			break;
	}

	return anguloEmGraus;
}

inline Ponto getVerticeMaisDistante(Ponto pInicio,Ponto p1,Ponto p2){
	double distancia1 = sqrt(pow(pInicio.x - p1.x,2) + pow(pInicio.z - p1.z,2));
	double distancia2 = sqrt(pow(pInicio.x - p2.x,2) + pow(pInicio.z - p2.z,2));
	return (distancia1 > distancia2)?p1:p2;
}

inline Ponto getVerticeComMaiorZ(list<Ponto> *vertices){
	Ponto verticeComMaiorZ;
	list<Ponto>::const_iterator iterator = vertices->begin();
	
	verticeComMaiorZ = *iterator;
	++iterator;

	while(iterator != vertices->end()){
		Ponto p = *iterator;		

		if (p.z > verticeComMaiorZ.z || (p.z == verticeComMaiorZ.z && p.x < verticeComMaiorZ.x)){ 
			verticeComMaiorZ = p;
		}		
		++iterator;
	}
	return verticeComMaiorZ;
};

inline Ponto getVerticeComMenorZ(list<Ponto> *vertices){
	Ponto verticeComMenorZ;
	list<Ponto>::const_iterator iterator = vertices->begin();
	
	verticeComMenorZ = *iterator;
	++iterator;

	while(iterator != vertices->end()){
		Ponto p = *iterator;		

		if (p.z < verticeComMenorZ.z || (p.z == verticeComMenorZ.z && p.x < verticeComMenorZ.x)){ 
			verticeComMenorZ = p;
		}		
		++iterator;
	}
	return verticeComMenorZ;
};



list<Ponto> ordenarVertices(list<Ponto> vertices)
{
	const Ponto verticeComMaiorZ = getVerticeComMaiorZ(&vertices);
	vertices.remove(verticeComMaiorZ);
	vertices.push_back(verticeComMaiorZ);

	Ponto verticeOrigem = verticeComMaiorZ;
	list<Ponto> verticesOrdenados;
	verticesOrdenados.push_back(verticeOrigem);
	
	EixoX eixoX = Positivo;
	const Ponto verticeComMenorZ = getVerticeComMenorZ(&vertices);

	while(true){
		Ponto verticeDestino;
		double angulo = 360;

		if (verticeOrigem == verticeComMenorZ)	eixoX = Negativo;

		list<Ponto>::const_iterator iterator = vertices.begin();
		while(iterator != vertices.end()){
			Ponto vertice = *iterator;
			double anguloEmGraus = calcularAlguloRelativoAoEixoX(verticeOrigem,vertice,eixoX);
			if (anguloEmGraus < angulo){
				verticeDestino = vertice;
				angulo = anguloEmGraus;
			}else if (anguloEmGraus == angulo){
				verticeDestino = getVerticeMaisDistante(verticeOrigem,vertice,verticeDestino);
			}

			++iterator;
		}
		if (verticeDestino == verticeComMaiorZ) break;

		verticeOrigem = verticeDestino;
		verticesOrdenados.push_back(verticeDestino);
		vertices.remove(verticeDestino); 
	}
	return verticesOrdenados;
}