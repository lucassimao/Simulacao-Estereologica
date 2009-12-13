#ifndef RENDERIZAR_INTERCEPTOS_STATE
#define RENDERIZAR_INTERCEPTOS_STATE

#include "RenderizacaoState.h"
#include "..\..\model\SimulacaoCaixa.h"
#include "..\drawVisitor\InterceptoDesignerVisitor.h"

using namespace simulacao::model;
using namespace simulacao::canvas::drawVisitor;

namespace simulacao{
	namespace canvas{
		namespace glWidget{

			class RenderizarInterceptosState: public RenderizacaoState{
			private:
				InterceptoDesignerVisitor *visitor;
			public:
				RenderizarInterceptosState();
				virtual void draw(SimulacaoCaixa *simulacao) const;

			};

		}
	}
}

#endif