#include <QDebug>
#include <cmath>
#include "GL\glut.h"

#include "..\..\model\interceptos\Poligono.h"
#include "..\..\model\interceptos\Disco.h"
using namespace simulacao::model::interceptos;

#include "..\..\utils\SegmentoDeReta.h"

#include "InterceptoDeAreaDrawVisitor.h"
using namespace simulacao::canvas::drawVisitor;

InterceptoDeAreaDrawVisitor::InterceptoDeAreaDrawVisitor(){

}

inline void InterceptoDeAreaDrawVisitor::visit(Disco *disco){
	Ponto centro = disco->centro;
	double raio = disco->raio;

	int circlePoints=100;
	double angulo=0;
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_CULL_FACE); 
	glCullFace(GL_FRONT); 
	glDisable(GL_LIGHTING);
	Cor c = disco->getCor();
	glColor4f(c.r,c.g,c.b,1);

	glBegin(GL_POLYGON);
	for (int i=0;  i < circlePoints; i++)  { 
		angulo =  2*3.141592*i/circlePoints;
		glVertex3f(centro.x + raio*cos(angulo) ,centro.y,centro.z + raio*sin(angulo));	
	}
	glEnd();	
	glPopAttrib();

}

inline void InterceptoDeAreaDrawVisitor::visit(Poligono *poligono){
	assert(poligono->getVertices().size()>=3);

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_CULL_FACE); 
	//glCullFace(GL_FRONT); 
	glDisable(GL_LIGHTING);
	
	
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glHint(GL_LINE_SMOOTH_HINT | GL_POLYGON_SMOOTH_HINT,  GL_DONT_CARE);
	
	Cor c = poligono->getCor();
	glColor4f(c.r,c.g,c.b,1.0f);
	glLineWidth(.1);

	
	glBegin(GL_POLYGON);
	{
		list<Ponto> vertices1 = poligono->getVertices();
		list<Ponto>::const_iterator iterator = vertices1.begin();

		while(iterator != vertices1.end() ){
			Ponto p = *iterator;
			glVertex3f(p.x,p.y,p.z);
			++iterator;
		}

	}
	glEnd();
	
	/*
	//glPointSize(7);
	glLineWidth(3);	
	list<Ponto> vertices = poligono->getVertices();
	list<Ponto>::const_iterator iterator = vertices.begin();

	assert(vertices.size()>=3);

	glBegin(GL_LINE_LOOP);
		while(iterator != vertices.end() ){
			Ponto p = *iterator;
			glColor3f(1,0,0);
			glVertex3f(p.x,p.y,p.z);
			++iterator;
		}
	glEnd();
	*/	
	
	


	glPopAttrib();
}

