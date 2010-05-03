#ifndef EXPORTADOR_DE_CORTES_SISTEMATICOS
#define EXPORTADOR_DE_CORTES_SISTEMATICOS

#include "..\model\SimulacaoCaixa.h"
#include "..\sqlite3\sqlite3.h"


using simulacao::model::SimulacaoCaixa;

namespace simulacao{

	namespace math{

		class ExportadorDeCortesSistematicos{
			private:
				sqlite3 *db;				
				int qtdePlanos;
				SimulacaoCaixa *simulacao;
			public:
				ExportadorDeCortesSistematicos(const char* bancoDeDados, int qtdePlanos,SimulacaoCaixa *simulacao);
				void exportar();
		
		};

	}

}

#endif	EXPORTADOR_DE_CORTES_SISTEMATICOS