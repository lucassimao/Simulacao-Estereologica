#ifndef RENDERIZAR_INTERCEPTOS_STATE
#define RENDERIZAR_INTERCEPTOS_STATE

#include <vector>
#include "RenderizacaoStrategy.h"
#include "..\..\model\SimulacaoCaixa.h"
#include "..\..\model\interceptos\Intercepto.h"
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
				vector<Intercepto*> *interceptos;
				InterceptoDeAreaDrawVisitor *interceptoDeAreaDrawVisitor;
				InterceptoLinearDrawVisitor *interceptoLinearDrawVisitor;
				Grade *grade;

				inline void coletarInterceptos(SimulacaoCaixa *simulacao);
			public:
				RenderizarInterceptosStrategy(Grade *);
				inline void draw(SimulacaoCaixa *simulacao);
				virtual inline RenderizacaoStrategyType getTypeOf(){ return RenderizarInterceptos;}

				vector<Intercepto*> * getInterceptos(){ return this->interceptos; };

			};

		}
	}
}

#endif