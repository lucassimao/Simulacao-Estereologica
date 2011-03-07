#ifndef RENDERIZAR_INTERCEPTOS_STATE
#define RENDERIZAR_INTERCEPTOS_STATE

#include <vector>
#include "RenderizacaoStrategy.h"
#include "..\..\model\SimulacaoCaixa.h"
#include "..\..\model\interceptos\InterceptoDeArea.h"
#include "..\..\model\grade\Grade.h"
#include "..\drawVisitor\InterceptoDeAreaDrawVisitor.h"
#include "..\drawVisitor\InterceptoLinearDrawVisitor.h"
#include "..\..\defs.h"


using namespace simulacao::model;
using namespace simulacao::canvas::drawVisitor;
using namespace simulacao::model::interceptos;
using namespace simulacao::model::grade;
using std::vector;

namespace simulacao{
	namespace canvas{
		namespace glWidget{

			class RenderizarInterceptosStrategy: public RenderizacaoStrategy{
			private:
				vector<InterceptoDeArea*> *interceptos;
				InterceptoDeAreaDrawVisitor *interceptoDeAreaDrawVisitor;
				InterceptoLinearDrawVisitor *interceptoLinearDrawVisitor;
				Grade *grade;

				inline void coletarInterceptos(SimulacaoCaixa *simulacao);
			public:
				RenderizarInterceptosStrategy(Grade *);
				inline void draw(SimulacaoCaixa *simulacao);
				virtual inline RenderizacaoStrategyType getTypeOf(){ return RenderizarInterceptos;}

				vector<InterceptoDeArea*> * getInterceptos(){ return this->interceptos; };

			};

		}
	}
}

#endif