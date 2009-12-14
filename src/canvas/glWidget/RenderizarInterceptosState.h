#ifndef RENDERIZAR_INTERCEPTOS_STATE
#define RENDERIZAR_INTERCEPTOS_STATE

#include <vector>
#include "RenderizacaoState.h"
#include "..\..\model\SimulacaoCaixa.h"
#include "..\drawVisitor\InterceptoDesignerVisitor.h"
#include "..\..\model\interceptos\Intercepto.h"


using namespace simulacao::model::interceptos;
using namespace simulacao::model;
using namespace simulacao::canvas::drawVisitor;
using std::vector;

namespace simulacao{
	namespace canvas{
		namespace glWidget{

			class RenderizarInterceptosState: public RenderizacaoState{
			private:
				vector<Intercepto*> *interceptos;
				InterceptoDesignerVisitor *visitor;
			public:
				RenderizarInterceptosState();
				virtual inline void draw(SimulacaoCaixa *simulacao) const;

			};

		}
	}
}

#endif