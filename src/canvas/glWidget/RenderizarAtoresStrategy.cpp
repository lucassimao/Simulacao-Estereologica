#include <GL\glut.h>
#include <QDebug>
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
	PlanoDeCorte *planoDeCorte = simulacao->getPlanoDeCorte();
	
	while (nbActors--)
	{
		NxActor* actor = *actors++;
		Ator *ator = static_cast<Ator*>(actor->userData);


		if (ator){
			Cor cor = ator->cor;
			glPushAttrib(GL_ALL_ATTRIB_BITS);

			if (planoDeCorte && ator->estaInterceptadoPeloPlano(planoDeCorte->getNxActor()->getGlobalPosition()))
				glColor4f(1.0f, 0, 0, 1.0f);
			else 
				glColor4f(cor.r,cor.g,cor.b,1.0f);
			
			DrawActor(actor, NULL, false);
			glPopAttrib();
		}
		else
		{
			DrawActor(actor, NULL, false);	
		}
				
	}	
}