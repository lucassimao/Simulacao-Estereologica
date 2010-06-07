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
				void exportarParaArquivo();

				static int exportarPlanoCallback(void *instancia, int qtdeColunas, char **colunas, char **nomeColunas);
				static int processarPoligonos(ExportadorDeCortesSistematicos *obj, int plano_pk);
				static int processarDiscos(ExportadorDeCortesSistematicos *obj, int plano_pk);
				static double calcularPerimetroPoligono(int poligono_pk,sqlite3 *db);

			public:
				ExportadorDeCortesSistematicos(const char* diretorio, const char* bancoDeDados, int qtdePlanos,SimulacaoCaixa *simulacao);
				void exportar();
		
		};

	}

}

#endif	EXPORTADOR_DE_CORTES_SISTEMATICOS