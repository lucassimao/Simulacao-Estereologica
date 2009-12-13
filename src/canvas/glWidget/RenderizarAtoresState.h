#ifndef RENDERIZAR_ATORES_STATE
#define RENDERIZAR_ATORES_STATE

#include "RenderizacaoState.h"
#include "..\..\model\SimulacaoCaixa.h"

using namespace simulacao::model;


namespace simulacao{
	namespace canvas{
		namespace glWidget{

			class RenderizarAtoresState: public RenderizacaoState{
			public:
				RenderizarAtoresState();
				virtual inline void draw(SimulacaoCaixa *simulacao) const;

			};

		}
	}
}

#endif