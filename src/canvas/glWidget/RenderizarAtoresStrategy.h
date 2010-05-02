#ifndef RENDERIZAR_ATORES_STATE
#define RENDERIZAR_ATORES_STATE

#include "RenderizacaoStrategy.h"
#include "..\..\model\SimulacaoCaixa.h"
#include "..\..\defs.h"
				
using namespace simulacao::model;


namespace simulacao{
	namespace canvas{
		namespace glWidget{

			class RenderizarAtoresStrategy: public RenderizacaoStrategy{
			public:
				RenderizarAtoresStrategy();
				virtual inline RenderizacaoStrategyType getTypeOf(){ return RenderizarAtores;}
				virtual inline void draw(SimulacaoCaixa *simulacao) ;

			};

		}
	}
}

#endif