#include <vector>
using std::vector;

#include <cmath>

#include "InterceptoLinearDrawVisitor.h"
using namespace simulacao::canvas::drawVisitor;

#include "GL\glut.h"

#include "..\..\model\grade\Grade.h"
#include "..\..\model\grade\RetaDeTeste.h"

using namespace simulacao::model::grade;


#include "..\..\model\interceptos\Poligono.h"
#include "..\..\model\interceptos\Disco.h"
using namespace simulacao::model::interceptos;

#include "..\..\utils\SegmentoDeReta.h"


InterceptoLinearDrawVisitor::InterceptoLinearDrawVisitor(Grade *g){
	this->grade = g;
}

inline void InterceptoLinearDrawVisitor::visit(Disco *disco){
	double z0 = disco->centro.z + disco->raio;
	double z1 = disco->centro.z - disco->raio;

	vector<RetaDeTeste> linhas = this->grade->getLinhasNoIntervalo(z0,z1);
	vector<RetaDeTeste>::const_iterator iterator = linhas.begin();

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	
	glEnable(GL_CULL_FACE); 
	glCullFace(GL_FRONT); 
	glDisable(GL_LIGHTING);
	glColor3f(1.0f,1.0f,1.0f);
	glLineWidth(2.5f);

	glBegin(GL_LINES);
		while(iterator!=linhas.end()){
			RetaDeTeste l = *iterator;
			
			double distanciaEntreALinhaEOCentroDoDisco = abs(disco->centro.z - l.linhaInicio.z);
			double comprimentoDoInterceptoLinear = 2*sqrt(pow(disco->raio,2)-pow(distanciaEntreALinhaEOCentroDoDisco,2));
			
			Ponto interceptoInicio = {disco->centro.x - comprimentoDoInterceptoLinear/2.0,l.linhaInicio.y,l.linhaInicio.z};
			Ponto interceptoFim = {disco->centro.x + comprimentoDoInterceptoLinear/2.0,l.linhaInicio.y,l.linhaInicio.z};

			glVertex3f(interceptoInicio.x,interceptoInicio.y,interceptoInicio.z);
			glVertex3f(interceptoFim.x,interceptoFim.y,interceptoFim.z);

			++iterator;
		}
	glEnd();

	glPopAttrib();
}

inline void InterceptoLinearDrawVisitor::visit(Poligono *poligono){

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	
	glEnable(GL_CULL_FACE); 
	glCullFace(GL_FRONT); 
	glDisable(GL_LIGHTING);
	glColor3f(1.0f,1.0f,1.0f);
	glLineWidth(2.5f);

	glBegin(GL_LINES);
		vector<InterceptoLinear*> interceptosLineares = poligono->getInterceptosLineares(this->grade);
		if (interceptosLineares.size() > 0){
			vector<InterceptoLinear*>::const_iterator iterator = interceptosLineares.begin();
			while(iterator != interceptosLineares.end()){
				InterceptoLinear* intercepto = *iterator;
				glVertex3f(intercepto->p0.x,intercepto->p0.y,intercepto->p0.z);
				glVertex3f(intercepto->p1.x,intercepto->p1.y,intercepto->p1.z);
				iterator++;
			}
		}
	glEnd();

	glPopAttrib();
}