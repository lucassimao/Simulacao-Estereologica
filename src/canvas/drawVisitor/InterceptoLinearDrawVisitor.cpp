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
	glColor3f(0,0,0);
	glLineWidth(1.5f);

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
	double z0 = poligono->verticeComMaiorZ.z;
	double z1 = poligono->verticeComMenorZ.z;
	
	vector<RetaDeTeste> linhas = this->grade->getLinhasNoIntervalo(z0,z1);
	vector<RetaDeTeste>::const_iterator iterator = linhas.begin();

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	
	glEnable(GL_CULL_FACE); 
	glCullFace(GL_FRONT); 
	glDisable(GL_LIGHTING);
	glColor3f(0,0,0);
	glLineWidth(1.5f);

	glBegin(GL_LINES);
		while(iterator!=linhas.end()){
			RetaDeTeste retaDeTeste = *iterator;
			
			// já me retorna as arestas que estão interceptadas pela reta de teste
			list<SegmentoDeReta> arestasInterceptadas = poligono->getArestasInterceptadas(retaDeTeste);
			
			list<SegmentoDeReta>::const_iterator iter = arestasInterceptadas.begin();
			while(iter!=arestasInterceptadas.end()){
					SegmentoDeReta seg = *iter;
					Ponto p;
					seg.interceptar(retaDeTeste,&p);
					glVertex3f(p.x,p.y,p.z);
				++iter;
			}

			++iterator;
		}
	glEnd();

	glPopAttrib();
}