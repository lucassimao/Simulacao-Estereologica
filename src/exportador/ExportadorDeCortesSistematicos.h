#ifndef EXPORTADOR_DE_CORTES_SISTEMATICOS
#define EXPORTADOR_DE_CORTES_SISTEMATICOS

#include <QtDebug>
#include <sstream>
#include <stdexcept>
#include "..\sqlite3\sqlite3.h"
#include "..\model\SimulacaoCaixa.h"
#include "..\utils\GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h"
#include "..\utils\GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy.h"
#include "..\math\ColetorDeInterceptosLinearesVisitor.h"
#include "..\math\ColetorDeAreasVisitor.h"
#include "..\math\ColetorDePontosVisitor.h"
#include "..\utils\DAO.h"
#include "..\utils\DataBaseFactory.h"
#include "..\defs.h"
#include "..\model\Parametros.h"


using namespace simulacao::math;
using namespace simulacao::model;
using namespace simulacao::math::mathVisitor;
using namespace std;
using simulacao::model::SimulacaoCaixa;

namespace simulacao{

	namespace exportador{

		class ExportadorDeCortesSistematicos{
			private:				
				int qtdePlanos;
				SimulacaoCaixa *simulacao;

				sqlite3* newDb();

			public:
				ExportadorDeCortesSistematicos(int qtdePlanos,SimulacaoCaixa *simulacao);
				sqlite3* exportar();
	
		};

	}

}

#endif	EXPORTADOR_DE_CORTES_SISTEMATICOS