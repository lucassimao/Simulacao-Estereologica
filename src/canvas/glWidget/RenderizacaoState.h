#ifndef RENDERIZACAO_STATE
#define RENDERIZACAO_STATE

#include "..\..\model\SimulacaoCaixa.h"

using namespace simulacao::model;


namespace simulacao{
	namespace canvas{
		namespace glWidget{

			class RenderizacaoState{
			
			public:
				RenderizacaoState();
				virtual inline void draw(SimulacaoCaixa *simulacao) const = 0;

			};

		}
	}
}

#endif