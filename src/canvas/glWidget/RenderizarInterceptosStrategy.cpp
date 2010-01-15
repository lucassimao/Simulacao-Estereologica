#include <vector>
#include "RenderizarInterceptosStrategy.h"
#include "..\..\model\interceptos\Intercepto.h"
#include "..\..\model\atores\Ator.h"
#include "..\..\draw\DrawObjects.h"
#include "GL\glut.h"



using std::vector;
using namespace simulacao::canvas::glWidget;
using namespace simulacao::model::interceptos;
using namespace simulacao::model::atores;



RenderizarInterceptosStrategy::RenderizarInterceptosStrategy(){
	interceptos = new vector<Intercepto*>();
}

inline void RenderizarInterceptosStrategy::draw(SimulacaoCaixa *simulacao){		

	if (interceptos->size() == 0){
		coletarInterceptos(simulacao);
	}

	vector<Intercepto*>::const_iterator iterator = interceptos->begin();
	InterceptoDeAreaDrawVisitor *visitor = new InterceptoDeAreaDrawVisitor();
	
	while(iterator!=interceptos->end())
	{
		Intercepto *intercepto = *iterator;
		intercepto->accept(visitor);
		++iterator;
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
				for(int i=-9;i<10;++i){
					glVertex3f(-10,simulacao->getPlanoDeCorte()->getGlobalPosition().y,i);
					glVertex3f(10,simulacao->getPlanoDeCorte()->getGlobalPosition().y,i);
				}
			glEnd();
		}

		if (simulacao->getExibirPontosTeste()){
			glColor4f(0.0f,0.0f,1.0f,0);
			glBegin(GL_POINTS);
				for(int i=-9;i<10;++i){
					for(int j=-9;j<10;j++){
						Ponto p = {j,simulacao->getPlanoDeCorte()->getGlobalPosition().y,i};
						//if (renderizarPonto(p))
							glVertex3f(j,simulacao->getPlanoDeCorte()->getGlobalPosition().y,i);
					}
				}
			glEnd();	
		}
		
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