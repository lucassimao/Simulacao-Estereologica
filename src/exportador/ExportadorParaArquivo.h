#ifndef EXPORTADOR_PARA_ARQUIVO
#define EXPORTADOR_PARA_ARQUIVO

#include <QtDebug>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cassert>
#include "..\sqlite3\sqlite3.h"
#include "..\defs.h"
#include "..\model\interceptos\InterceptoLinear.h"

using namespace std;
using namespace simulacao::model::interceptos;

#define PI 3.1415926

namespace simulacao{
	namespace exportador{

		class ExportadorParaArquivo{
		
		public:
			ExportadorParaArquivo(string &destino, sqlite3* db);
			void exportarPlanosDeCorte();
			void exportarTabelasDeProbabilidade(int qtdeClassesDeIntercepto);
			void exportarInterceptosMedioParaPrisma();
			void exportarInterceptosMedioParaEsfera();

		private:
			string destino;
			sqlite3 *db;
			int qtdeClassesDeIntercepto;

			void exportarPlanoDeCorte(int plano_pk);
			void salvarAreaDosPoligonos(int plano_pk, ofstream &outFile);
			void salvarAreaDosDiscos(int plano_pk, ofstream &outFile);
			void salvarInterceptosLineares(int plano_pk, ofstream &outFile);
			void salvarQtdeDePontosInternos(int plano_pk, ofstream &outFile);
			void salvarInterceptosDePoro(int plano_pk);

			void exportarInterceptoDeAreaMedioParaPrisma();
			void exportarInterceptoLinearMedioParaPrisma();
			void exportarInterceptoDePerimetroMedioParaPrisma();

			void exportarInterceptoDeAreaMedioParaEsfera();
			void exportarInterceptoLinearMedioParaEsfera();
			void exportarInterceptoDePerimetroMedioParaEsfera();
		};
	}
}


#endif