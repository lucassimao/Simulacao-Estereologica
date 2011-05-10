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

		class ClasseDeGrao{			
		public:
			int qtdeDeGraosDaClasse;
			virtual double getDiametroEquivalente()  = 0;
		};

		class ClasseDeGraoEsferico : public ClasseDeGrao{
		public:
			double raio;

			double getDiametroEquivalente() { 
				return raio*2;
			};
		};

		class ClasseDeGraoPrismatico : public ClasseDeGrao{
		public:
			double L0,razaoDeAspecto, razaoDeTruncamento;

			double getDiametroEquivalente() { 
				double raizDe3 = sqrt(3.0);
				return pow( (3*raizDe3/(2*PI))*razaoDeAspecto*(1 - 3*pow(razaoDeTruncamento,2)), 1/3.0 ) * L0;
			};
		};

		typedef enum { Linear, Area, Perimetro,Poro} TipoDeIntercepto;

		enum TipoDeGrao{ Esferico,Prismatico };


		class ProcessadorDeClassesDeIntercepto{
		private:
			sqlite3 *db;
			TipoDeGrao tipoDeGrao;

			struct{
				bool operator()(ClasseDeGrao *c1, ClasseDeGrao *c2) const{
					return c1->getDiametroEquivalente() < c2->getDiametroEquivalente();
				}
			} ClasseDeGraoCmp;

			TipoDeGrao __getTipoDeGraoNaSimulacao();

			int getQuantidadeDeInterceptosEsfericosNoIntervalo(double subClasseMinima,double subClasseMaxima,ClasseDeGraoEsferico *classe,TipoDeIntercepto tipoDeIntercepto);
			double getMenorInterceptoEsferico(TipoDeIntercepto tipoDeIntercepto);
			double getMaiorInterceptoEsferico(TipoDeIntercepto tipoDeIntercepto);
			

			int getQuantidadeDeInterceptosPrismaticosNoIntervalo(double subClasseMinima,double subClasseMaxima,ClasseDeGraoPrismatico *classe,TipoDeIntercepto tipoDeIntercepto);
			double getMenorInterceptoPrismatico(TipoDeIntercepto tipoDeIntercepto);
			double getMaiorInterceptoPrismatico(TipoDeIntercepto tipoDeIntercepto);
			

			int getQuantidadeDeInterceptosPorosos(double limiteInferior, double limiteSuperior);
		public:
			ProcessadorDeClassesDeIntercepto(sqlite3 *db);
			// Esse método retorna as classes de grão presentes na simulação em ordem crescente de diâmetro equivalente
			vector<ClasseDeGrao*> getClassesDeGrao();
			int getQuantidadeDeInterceptosNoIntervalo(double subClasseMinima,double subClasseMaxima,ClasseDeGrao *classe,TipoDeIntercepto tipoDeIntercepto);
			double getMenorIntercepto(TipoDeIntercepto tipoDeIntercepto);
			double getMaiorIntercepto(TipoDeIntercepto tipoDeIntercepto);
			TipoDeGrao getTipoDeGraoNaSimulacao();
			
			vector<ClasseDeGrao*> getClassesDeGraoEsfericos();
			vector<ClasseDeGrao*> getClassesDeGraoPrismaticos();
			vector<vector<int>> gerarTabelaDeDistribuicaoDeInterceptos(TipoDeIntercepto tipoDeIntercepto, int qtdeDeClassesDeIntercepto);

		};
	}
}


#endif