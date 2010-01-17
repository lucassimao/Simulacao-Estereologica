#ifndef RENDERIZAR_INTERCEPTOS_STATE
#define RENDERIZAR_INTERCEPTOS_STATE

#include <vector>
#include "RenderizacaoStrategy.h"
#include "..\..\model\SimulacaoCaixa.h"
#include "..\..\model\interceptos\Intercepto.h"
#include "..\..\model\grade\Grade.h"


using namespace simulacao::model;
using namespace simulacao::model::interceptos;
using namespace simulacao::model::grade;
using std::vector;

namespace simulacao{
	namespace canvas{
		namespace glWidget{

			class RenderizarInterceptosStrategy: public RenderizacaoStrategy{
			private:
				vector<Intercepto*> *interceptos;
				Grade *grade;

				inline void coletarInterceptos(SimulacaoCaixa *simulacao);
				inline bool renderizarPonto(Ponto p);
			public:
				RenderizarInterceptosStrategy();
				inline void draw(SimulacaoCaixa *simulacao);

			};

		}
	}
}

#endif