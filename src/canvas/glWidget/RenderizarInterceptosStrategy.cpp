#include <iostream>
#include <fstream>
#include <vector>
#include "RenderizarInterceptosStrategy.h"
#include "GL\glut.h"
#include "..\..\model\interceptos\Intercepto.h"
#include "..\..\model\atores\Ator.h"
#include "..\..\draw\DrawObjects.h"
#include "..\..\math\ColetorDePontosVisitor.h"
#include "..\..\math\ColetorDeAreasVisitor.h"
#include "..\..\math\ColetorDeInterceptosLinearesVisitor.h"
#include "..\..\defs.h"

using namespace std;
using namespace simulacao::math::mathVisitor;
using namespace simulacao::canvas::glWidget;
using namespace simulacao::model::interceptos;
using namespace simulacao::model::atores;


RenderizarInterceptosStrategy::RenderizarInterceptosStrategy(Grade *grade){
	interceptos = new vector<Intercepto*>();
	this->grade = grade;
	interceptoDeAreaDrawVisitor = new InterceptoDeAreaDrawVisitor();
	interceptoLinearDrawVisitor = new InterceptoLinearDrawVisitor(this->grade);

}

inline void RenderizarInterceptosStrategy::draw(SimulacaoCaixa *simulacao){		

	if (interceptos->size() == 0){
		coletarInterceptos(simulacao);

		// coletando informações sobre as areas interceptadas e os segmento
		// das retas de teste internos aos interceptos de área
		ColetorDeAreasVisitor *visitor1 = new ColetorDeAreasVisitor(this->grade);
		ColetorDeInterceptosLinearesVisitor *visitor2 = new ColetorDeInterceptosLinearesVisitor(this->grade);
		ColetorDePontosVisitor *visitor3 = new ColetorDePontosVisitor(this->grade);

		vector<Intercepto*>::const_iterator  iter= this->interceptos->begin();
		locale myloc(locale(),new WithComma);
		
		while(iter!=interceptos->end()){
			Intercepto *i = *iter;
			i->accept(visitor1);
			i->accept(visitor2);
			i->accept(visitor3);
			++iter;
		}

		ofstream areas("areas.csv",std::ios::out);
		areas.imbue(myloc);
		
		for(int i=0;i<visitor1->getAreas().size();++i){
			double d = visitor1->getAreas().at(i);
			areas << d << endl;
		}
		areas.close();

		
		ofstream interceptosLineares("interceptosLineares.csv",std::ios::out);
		interceptosLineares.imbue(myloc);

		for(int i=0;i<visitor2->interceptosLineares.size();++i){
			double d = visitor2->interceptosLineares.at(i);
			interceptosLineares << d << endl;
		}
		interceptosLineares.close();

		ofstream qtdeDePontos("qtdePontos.txt",std::ios::out);
		Parametros *params = Parametros::getInstance();
		int qtdeTotalDePontos = params->getQtdePontosPorLinhaNaGrade() * params->getQtdeLinhasNaGrade();

		qtdeDePontos << "Quantidade de pontos na grade: " << qtdeTotalDePontos << endl;
		qtdeDePontos << "Quantidade de pontos internos às seções de área: " << visitor3->getQtdeDePontosInternosAInterceptosDeArea() << endl;

		qtdeDePontos.close();
		
		
	}
		
		// aqui inicia o desenho dos interceptos de area
		vector<Intercepto*>::const_iterator iterator = interceptos->begin();
		
		while(iterator!=interceptos->end())
		{
			Intercepto *intercepto = *iterator;
			
			intercepto->accept(this->interceptoDeAreaDrawVisitor);
			if (simulacao->getExibirRetasTeste())
				intercepto->accept(this->interceptoLinearDrawVisitor);

			++iterator;
		}

		if (simulacao->getExibirRetasTeste() || simulacao->getExibirPontosTeste()){
	
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glEnable(GL_CULL_FACE); 
			glCullFace(GL_FRONT); 
			glDisable(GL_LIGHTING);
			glColor4f(1.0f,0.0f,0.0f,0);
			glLineWidth(0.2);
			glPointSize(3.0f);


			if (simulacao->getExibirRetasTeste()){
				
				vector<RetaDeTeste>::const_iterator linhas = this->grade->getLinhasIterator();
				vector<RetaDeTeste>::const_iterator end = this->grade->getLinhasIteratorEnd();
				
				glBegin(GL_LINES);
					while(linhas!=end){
						RetaDeTeste l = *linhas;
						
						glVertex3f(l.linhaInicio.x,l.linhaInicio.y,l.linhaInicio.z);
						glVertex3f(l.linhaFim.x,l.linhaFim.y,l.linhaFim.z);
						
						linhas++;
					}	
				glEnd();
				
			}

		
			if (simulacao->getExibirPontosTeste()){
				glColor4f(0.0f,0.0f,1.0f,0);
				glBegin(GL_POINTS);
					vector<RetaDeTeste>::const_iterator linhas = this->grade->getLinhasIterator();
					vector<RetaDeTeste>::const_iterator end = this->grade->getLinhasIteratorEnd();

					while(linhas!=end){
						RetaDeTeste l = *linhas;

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
		
			glPopAttrib();	
	}
	if (simulacao->getPlanoDeCorte()){
		glPushAttrib(GL_ALL_ATTRIB_BITS);
			PlanoDeCorte *planoDeCorte = simulacao->getPlanoDeCorte();
			Cor cor = planoDeCorte->cor;
			glColor3f(cor.r,cor.g,cor.b);
			DrawActor(planoDeCorte->getNxActor(), NULL, false);
		glPopAttrib();
	}


}

inline void RenderizarInterceptosStrategy::coletarInterceptos(SimulacaoCaixa *simulacao){

		NxU32 qtdeAtores = simulacao->getQtdeObjetos();
		NxActor** atores = simulacao->getAtores();

		while (qtdeAtores--)
		{
			NxActor* ator = *atores++;

			if (ator != simulacao->getCaixa() && ator!= simulacao->getPlanoDeCorte()->getNxActor()){
				Ator *a = (Ator *)ator->userData;
				NxVec3 planoGlobalPosition = simulacao->getPlanoDeCorte()->getNxActor()->getGlobalPosition();
				Intercepto* i = a->getIntercepto(planoGlobalPosition);
				interceptos->push_back(i);					
			}
		}
}