#include <GL\glut.h>
#include <QDebug>
#include "RenderizarAtoresStrategy.h"
#include "..\..\draw\DrawObjects.h"
#include "..\..\model\atores\Ator.h"
#include "..\..\utils\GlobalPoseFactory.h"

using namespace simulacao::model::atores;
using namespace simulacao::canvas::glWidget;

RenderizarAtoresStrategy::RenderizarAtoresStrategy(){

}

inline void RenderizarAtoresStrategy::draw(SimulacaoCaixa *simulacao) {
	NxU32 nbActors = simulacao->getQtdeObjetos();
	NxActor** actors = simulacao->getAtores();
	PlanoDeCorte *planoDeCorte = simulacao->getPlanoDeCorte();
	NxActor *caixa = simulacao->getCaixa();
	
	while (nbActors--){
		NxActor* actor = *actors++;
		if (actor == caixa) continue;

		Ator *ator = static_cast<Ator*>(actor->userData);
		if (ator){
			Cor cor = ator->cor;
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			
			
			NxVec3 caixaGlobalPosition = simulacao->getCaixa()->getGlobalPosition(); 
			if (ator->estaForaDaCaixa(caixaGlobalPosition,Parametros::getInstance()->getArestaDaCaixa())){
				actor->setGlobalPosition(GlobalPoseFactory::getInstance()->newGlobalPosition());
			}

			if (planoDeCorte && ator->estaInterceptadoPeloPlano(planoDeCorte->getNxActor()->getGlobalPosition()))
				glColor4f(1.0f, 0, 0, 1.0f);
			else 
				glColor4f(cor.r,cor.g,cor.b,1.0f);
			
			glEnable(GL_LIGHTING);
			DrawActor(actor, NULL, false);
			glPopAttrib();
		
		}			
	}
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		Cor cor = planoDeCorte->cor;
		glColor3f(cor.r,cor.g,cor.b);
		DrawActor(planoDeCorte->getNxActor(), NULL, false);
	glPopAttrib();

	glDisable(GL_LIGHTING);
}