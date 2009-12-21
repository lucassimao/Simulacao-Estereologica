#include "RenderizarAtoresStrategy.h"
#include "..\..\draw\DrawObjects.h"

using namespace simulacao::canvas::glWidget;

RenderizarAtoresStrategy::RenderizarAtoresStrategy(){

}

inline void RenderizarAtoresStrategy::draw(SimulacaoCaixa *simulacao) {
	NxU32 nbActors = simulacao->getQtdeObjetos();
	NxActor** actors = simulacao->getAtores();

	while (nbActors--)
	{
		NxActor* actor = *actors++;
		DrawActor(actor, NULL, false);
	}
	
}