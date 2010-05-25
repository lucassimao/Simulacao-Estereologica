#include   <cstdio>
#include   <cmath>
#include   <cstdlib>
#include <QDebug>
#include <cassert>

#include <list>
using std::list;

#include <map>
using std::map;

#include "graham.h"


list<Ponto> ordenarVertices(list<Ponto> *vertices)
{
	
	list<Ponto>::const_iterator iterator = vertices->begin();
	
	Ponto verticeComMaiorZ= *iterator;
	++iterator;
	
	// procurando o vertice mais à frente
	while(iterator!=vertices->end()){
		Ponto p = *iterator;
		if (p.z > verticeComMaiorZ.z)
			verticeComMaiorZ = p;
		++iterator;
	}

	map<double,Ponto> pontosEAngulos; 
	list<Ponto>::const_iterator iterator2 = vertices->begin();

	while(iterator2!=vertices->end()){
		Ponto p = *iterator2;
		if (p.z != verticeComMaiorZ.z){
			double seno = abs(p.z - verticeComMaiorZ.z)/sqrt(pow(p.x - verticeComMaiorZ.x,2.0) + pow(p.z - verticeComMaiorZ.z,2.0));
			double angulo = asin(seno)*(180/3.1415926535);
			assert(angulo>0);
			if (p.x < verticeComMaiorZ.x)
				angulo = 180 - angulo;
			pontosEAngulos[angulo] = p;
		}
		++iterator2;
	}

	list<Ponto> verticesOrdenados;
	verticesOrdenados.push_back(verticeComMaiorZ);

	map<double,Ponto>::const_iterator iterator3 = pontosEAngulos.begin();
	while(iterator3!=pontosEAngulos.end()){
		double angulo = (*iterator3).first;
		Ponto p = (*iterator3).second;
		verticesOrdenados.push_back(p);
		++iterator3;
	}


	return verticesOrdenados;

}