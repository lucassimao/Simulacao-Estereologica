#include <GL\glut.h>
#include "RenderizarAtoresStrategy.h"
#include "..\..\draw\DrawObjects.h"
#include "..\..\model\atores\Ator.h"

using namespace simulacao::model::atores;
using namespace simulacao::canvas::glWidget;

RenderizarAtoresStrategy::RenderizarAtoresStrategy(){

}

inline void RenderizarAtoresStrategy::draw(SimulacaoCaixa *simulacao) {
	NxU32 nbActors = simulacao->getQtdeObjetos();
	NxActor** actors = simulacao->getAtores();
	
	
	while (nbActors--)
	{
		NxActor* actor = *actors++;
		Ator *ator = static_cast<Ator*>(actor->userData);
		
		
		if (ator){
			glPushAttrib(GL_ALL_ATTRIB_BITS);

			if (ator->estaInterceptadoPeloPlano(simulacao->getPlanoDeCorte()->getNxActor()->getGlobalPosition()))
				glColor4f(1.0, 0, 0, 1);
			else 
				glColor4f(ator->cor.r,ator->cor.g,ator->cor.b,1);
			
			DrawActor(actor, NULL, false);
			glPopAttrib();
		}
		else
		{
			DrawActor(actor, NULL, false);	
		}
		
		
	}
	
}