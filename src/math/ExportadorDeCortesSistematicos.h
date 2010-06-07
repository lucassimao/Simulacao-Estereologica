#ifndef EXPORTADOR_DE_CORTES_SISTEMATICOS
#define EXPORTADOR_DE_CORTES_SISTEMATICOS

#include <QtDebug>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include "..\sqlite3\sqlite3.h"
#include "..\model\SimulacaoCaixa.h"
#include "..\utils\GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h"
#include "..\utils\GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy.h"
#include "..\math\ColetorDeInterceptosLinearesVisitor.h"
#include "..\math\ColetorDeAreasVisitor.h"
#include "..\math\ColetorDePontosVisitor.h"
#include "..\utils\DAO.h"
#include "..\defs.h"
#include "..\model\Parametros.h"

using namespace simulacao::math;
using namespace simulacao::model;
using namespace simulacao::math::mathVisitor;
using namespace std;
using simulacao::model::SimulacaoCaixa;

namespace simulacao{

	namespace math{

		class ExportadorDeCortesSistematicos{
			private:
				string diretorio;
				sqlite3 *db;				
				int qtdePlanos;
				SimulacaoCaixa *simulacao;
				void exportarParaArquivo();

				void exportarPlano(int plano_pk);
				void salvarAreaDosPoligonos(int plano_pk, ofstream &outFile);
				void salvarAreaDosDiscos(int plano_pk, ofstream &outFile);
				void salvarInterceptosLineares(int plano_pk, ofstream &outFile);

			public:
				ExportadorDeCortesSistematicos(string &diretorio, const char* bancoDeDados, int qtdePlanos,SimulacaoCaixa *simulacao);
				void exportar();
		
		};

	}

}

#endif	EXPORTADOR_DE_CORTES_SISTEMATICOS