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

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_CULL_FACE); 
	glCullFace(GL_FRONT); 
	glDisable(GL_LIGHTING);
	
	
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
	glHint(GL_LINE_SMOOTH_HINT | GL_POLYGON_SMOOTH_HINT,  GL_DONT_CARE);
	
	Cor c = poligono->getCor();
	glColor4f(c.r,c.g,c.b,1.0f);
	glLineWidth(.1);
	glPointSize(7);
	Cor cores[] = {{1.0,0,0},{0,1.0,0},{0,0,1.0},{1.0,1.0,1.0},{0,0,0}};
	int i=0;

	glBegin(GL_POLYGON);
	{
		list<Ponto> vertices = poligono->getVertices();
		list<Ponto>::const_iterator iterator = vertices.begin();

		while(iterator != vertices.end() ){
				Ponto p = *iterator;
				glVertex3f(p.x,p.y,p.z);
				++iterator;
		}
	}
	glEnd();	

	glPopAttrib();
}

