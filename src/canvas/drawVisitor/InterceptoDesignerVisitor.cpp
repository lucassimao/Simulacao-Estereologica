#include <QDebug>
#include <cmath>
#include "GL\glut.h"

#include "..\..\model\interceptos\Polilinha.h"
#include "..\..\model\interceptos\Disco.h"
using namespace simulacao::model::interceptos;

#include "..\..\utils\SegmentoDeReta.h"

#include "InterceptoDesignerVisitor.h"
using namespace simulacao::canvas::drawVisitor;

InterceptoDesignerVisitor::InterceptoDesignerVisitor(){

}

inline void InterceptoDesignerVisitor::visit(Disco *disco){
	Ponto centro = disco->getCentro();
	double raio = disco->getRaio();

	int circlePoints=100;
	double angulo=0;
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_CULL_FACE); 
	glCullFace(GL_FRONT); 
	glColor4f(1.0f,0,0.3f,1);

	glBegin(GL_POLYGON);
	for (int i=0;  i < circlePoints; i++)  { 
		angulo =  2*3.141592*i/circlePoints;
		glVertex3f(centro.x + raio*cos(angulo) ,centro.y,centro.z + raio*sin(angulo));	
	}
	glEnd();	
	glPopAttrib();

}

inline void InterceptoDesignerVisitor::visit(Poligono *poligono){
	qDebug() << "Atencao mais um Poligono hein!\n";
}

inline void InterceptoDesignerVisitor::visit(Polilinha *p){
	vector<SegmentoDeReta> segs = p->vertices;
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_CULL_FACE); 
	glCullFace(GL_FRONT); 
	glColor4f(1.0f,0,0.3f,1);
	glLineWidth(3);

	glBegin(GL_LINES);
		vector<SegmentoDeReta>::const_iterator i = p->vertices.begin();
		while(i!=p->vertices.end()){
			SegmentoDeReta s = *i;
				glVertex3f(s.r0.x,s.r0.y,s.r0.z);
				glVertex3f(s.r1.x,s.r1.y,s.r1.z);
			++i;
		}
	glEnd();	
	glPopAttrib();	


}