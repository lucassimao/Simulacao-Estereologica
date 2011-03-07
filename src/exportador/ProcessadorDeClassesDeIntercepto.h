#ifndef PROCESSADOR_DE_CLASSES_DE_INTERCEPTO_H
#define PROCESSADOR_DE_CLASSES_DE_INTERCEPTO_H

#include <QtDebug>
#include <stdexcept>
#include <vector>
#include <cassert>
#include <cmath>
#include "..\sqlite3\sqlite3.h"
#include "..\defs.h"

using namespace std;

#define PI 3.1415926

namespace simulacao{

	namespace exportador{

		typedef struct{
			double L0,razaoDeAspecto, razaoDeTruncamento;
			
			double getDiametroEquivalente() { 
				double raizDe3 = sqrt(3.0);
				return pow( (3*raizDe3/2*PI)*razaoDeAspecto*(1 - 3*pow(razaoDeTruncamento,2)), 1/3.0 ) * L0;
			};
		}ClasseDeGrao;

		typedef enum { Linear, Area, Perimetro} TipoDeIntercepto;

		class ProcessadorDeClassesDeIntercepto{
		private:
			sqlite3 *db;
		public:
			ProcessadorDeClassesDeIntercepto(sqlite3 *db);
			// Esse método retorna as classes de grão presentes na simulação em ordem crescente de diâmetro equivalente
			vector<ClasseDeGrao> getClassesDeGrao();
			int getQuantidadeDeInterceptosNoIntervalo(double subClasseMinima,double subClasseMaxima,ClasseDeGrao classe,TipoDeIntercepto tipoDeIntercepto);
			double getMenorIntercepto(TipoDeIntercepto tipoDeIntercepto);
			double getMaiorIntercepto(TipoDeIntercepto tipoDeIntercepto);
		};
	}
}


#endif