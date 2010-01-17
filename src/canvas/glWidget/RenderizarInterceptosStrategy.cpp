#include <vector>
#include "RenderizarInterceptosStrategy.h"
#include "..\drawVisitor\InterceptoDeAreaDrawVisitor.h"
#include "..\drawVisitor\InterceptoLinearDrawVisitor.h"
#include "..\..\model\interceptos\Intercepto.h"
#include "..\..\model\atores\Ator.h"
#include "..\..\draw\DrawObjects.h"
#include "GL\glut.h"



using std::vector;
using namespace simulacao::canvas::glWidget;
using namespace simulacao::canvas::drawVisitor;
using namespace simulacao::model::interceptos;
using namespace simulacao::model::atores;



RenderizarInterceptosStrategy::RenderizarInterceptosStrategy(){
	interceptos = new vector<Intercepto*>();
}

inline void RenderizarInterceptosStrategy::draw(SimulacaoCaixa *simulacao){		

	if (interceptos->size() == 0){
		coletarInterceptos(simulacao);
		float alturaGrade = simulacao->getPlanoDeCorte()->getGlobalPosition().y;
		this->grade = new Grade(9,-18,alturaGrade,10,50);
	}

	vector<Intercepto*>::const_iterator iterator = interceptos->begin();
	InterceptoDeAreaDrawVisitor *visitor = new InterceptoDeAreaDrawVisitor();
	
	while(iterator!=interceptos->end())
	{
		Intercepto *intercepto = *iterator;
		intercepto->accept(visitor);
		++iterator;
	}

	
	vector<Intercepto*>::const_iterator iterator2 = interceptos->begin();
	InterceptoLinearDrawVisitor *visitor2 = new InterceptoLinearDrawVisitor(this->grade);
	
	while(iterator2!=interceptos->end())
	{
		Intercepto *intercepto = *iterator2;
		intercepto->accept(visitor2);
		++iterator2;
	}

	if (simulacao->getExibirRetasTeste() || simulacao->getExibirPontosTeste()){
	
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_CULL_FACE); 
		glCullFace(GL_FRONT); 
		glDisable(GL_LIGHTING);
		glColor4f(1.0f,0.0f,0.0f,0);
		glLineWidth(0.5f);
		glPointSize(2.9f);

		if (simulacao->getExibirRetasTeste() ){
			glBegin(GL_LINES);
				vector<Linha>::const_iterator linhas = this->grade->getLinhasIterator();
				vector<Linha>::const_iterator end = this->grade->getLinhasIteratorEnd();

				while(linhas!=end){
					Linha l = *linhas;
					glVertex3f(l.linhaInicio.x,l.linhaInicio.y,l.linhaInicio.z);
					glVertex3f(l.linhaFim.x,l.linhaFim.y,l.linhaFim.z);
					linhas++;
				}
				
			glEnd();
		}

		/*
		if (simulacao->getExibirPontosTeste()){
			glColor4f(0.0f,0.0f,1.0f,0);
			glBegin(GL_POINTS);
				vector<Linha>::const_iterator linhas = this->grade->getLinhasIterator();
				vector<Linha>::const_iterator end = this->grade->getLinhasIteratorEnd();

				while(linhas!=end){
					Linha l = *linhas;

					vector<Ponto>::const_iterator pontos = l.getPontosIterator();
					vector<Ponto>::const_iterator end = l.getPontosIteratorEnd();

					while(pontos!=end){
						Ponto p = *pontos;
						glVertex3f(p.x,p.y,p.z);
						++pontos;
					}

					linhas++;
				}
			glEnd();	
		}
		*/
		
		glPopAttrib();	
	}

	DrawActor(simulacao->getPlanoDeCorte(), NULL, false);


}

inline bool RenderizarInterceptosStrategy::renderizarPonto(Ponto p){
	
	vector<Intercepto*>::const_iterator iterator = interceptos->begin();

	while(iterator!=interceptos->end())
	{
		Intercepto *intercepto = *iterator;
		if (intercepto->contemPonto(p))
			return true;
		else
			++iterator;
	}
	return false;

}

inline void RenderizarInterceptosStrategy::coletarInterceptos(SimulacaoCaixa *simulacao){
		NxU32 qtdeAtores = simulacao->getQtdeObjetos();
		NxActor** atores = simulacao->getAtores();

		while (qtdeAtores--)
		{
			NxActor* ator = *atores++;

			{
				if (ator != simulacao->getCaixa() && ator!= simulacao->getPlanoDeCorte()){
					Ator *a = (Ator *)ator->userData;
					NxVec3 planoGlobalPosition = simulacao->getPlanoDeCorte()->getGlobalPosition();
					interceptos->push_back(a->getIntercepto(planoGlobalPosition));				
					
					// remove o ator da simulacao, agora nos interessa apenas os inteceptos
					simulacao->releaseActor(*ator);
					a = NULL;
				}

			}

		}
}