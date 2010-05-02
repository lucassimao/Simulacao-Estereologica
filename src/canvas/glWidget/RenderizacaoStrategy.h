#ifndef RENDERIZACAO_STATE
#define RENDERIZACAO_STATE

#include "..\..\model\SimulacaoCaixa.h"
#include "..\..\defs.h"
	
using namespace simulacao::model;


namespace simulacao{
	namespace canvas{
		namespace glWidget{

			class RenderizacaoStrategy{
			
			public:
				RenderizacaoStrategy();
				virtual inline void draw(SimulacaoCaixa *simulacao) = 0;
				virtual inline RenderizacaoStrategyType getTypeOf() = 0;

			};

		}
	}
}

#endif