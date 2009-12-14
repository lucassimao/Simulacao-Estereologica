#include <vector>
#include "RenderizarInterceptosState.h"
#include "..\..\model\interceptos\Intercepto.h"
#include "..\..\model\atores\Ator.h"
#include "..\..\draw\DrawObjects.h"


using std::vector;
using namespace simulacao::canvas::glWidget;
using namespace simulacao::model::interceptos;
using namespace simulacao::model::atores;



RenderizarInterceptosState::RenderizarInterceptosState(){
	this->interceptos = new vector<Intercepto*>();

}

inline void RenderizarInterceptosState::draw(SimulacaoCaixa *simulacao) const{
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
				simulacao->releaseActor(*ator);
				a = NULL;
			}

		}

	}
	vector<Intercepto*>::const_iterator iterator = interceptos->begin();
	InterceptoDesignerVisitor *visitor = new InterceptoDesignerVisitor();
	
	
	while(iterator!=interceptos->end())
	{
		Intercepto *intercepto = *iterator;
		intercepto->accept(visitor);
		++iterator;
	}
	
	DrawActor(simulacao->getPlanoDeCorte(), NULL, false);
}