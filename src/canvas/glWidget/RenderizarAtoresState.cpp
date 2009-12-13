#include "RenderizarAtoresState.h"
#include "..\..\draw\DrawObjects.h"

using namespace simulacao::canvas::glWidget;

RenderizarAtoresState::RenderizarAtoresState(){

}

void RenderizarAtoresState::draw(SimulacaoCaixa *simulacao) const{
	NxU32 nbActors = simulacao->getQtdeObjetos();
	NxActor** actors = simulacao->getAtores();

	while (nbActors--)
	{
		NxActor* actor = *actors++;
		DrawActor(actor, NULL, false);
	}
	
}