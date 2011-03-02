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
		}ClasseDeIntercepto;

		class ProcessadorDeClassesDeIntercepto{
		private:
			sqlite3 *db;
		public:
			ProcessadorDeClassesDeIntercepto(sqlite3 *db);
			// Esse método retorna as classes de intercepto presentes na simulação,
			// ordenada de forma crescente pelo seu diâmetro equivalente
			// Ou seja: a ordem das classes retornadas é dada do menor p/ o maior conforme o diâmetro equivalente da classe
			vector<ClasseDeIntercepto> getClassesDeIntercepto();
			int getQuantidadeDeInterceptosDeAreaNoIntervalo(double subClasseMinima,double subClasseMaxima,ClasseDeIntercepto classe);
			double getMenorInterceptoDeArea();
			double getMaiorInterceptoDeArea();
		};
	}
}


#endif