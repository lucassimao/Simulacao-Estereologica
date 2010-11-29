#ifndef H_PARAMETROS
#define H_PARAMETROS

#include <stdexcept>
#include "..\utils\Observable.h"
#include "..\defs.h"

using std::runtime_error;
using namespace simulacao::utils;


namespace simulacao{

	namespace model{
		typedef struct {
			double razaoDeAspecto;
			double razaoDeTruncamento;
			double L0;
			double altura;
			Cor cor;
		}PrismaTriangularTruncadoParams;

		typedef struct {
			double altura;
			double base;
			Cor cor;
		}PrismaTriangularParams;

		typedef struct {
			int qtdeLinhas;
			int qtdePontos; // pontos por linha
			bool pontosAleatorios;
		}GradeParams;

		typedef struct {
			double raio;
			Cor cor;
		} EsferaParams;

		class Parametros : public Observable
		{
		private:
			Parametros();
			static Parametros* instance;

			double arestaDaCaixa;
			PrismaTriangularParams prismaTriangular;
			PrismaTriangularTruncadoParams prismaTriangularTruncado;
			GradeParams grade;
			EsferaParams esfera;
			


		public:
			static Parametros* getInstance(){
				if (!Parametros::instance)
					instance = new Parametros();

				return Parametros::instance;
			}

			inline int  getQtdeLinhasNaGrade(){
				return this->grade.qtdeLinhas;
			}
			inline int getQtdePontosPorLinhaNaGrade(){
				return this->grade.qtdePontos;
			}
			
			inline void setParametrosDaGrade(int qtdeLinhas,int qtdePontos, bool pontosAleatorios){ 
				this->grade.qtdeLinhas= qtdeLinhas;
				this->grade.qtdePontos= qtdePontos;
				this->grade.pontosAleatorios= pontosAleatorios;
				notifyObservers();
			}
			inline GradeParams getParametrosDaGrade() const{
				return this->grade;
			}

			inline double getCentroDeMassaDaCaixa(){ return 10.0; }

			inline double getAlturaDaBaseDaCaixa(){ return getCentroDeMassaDaCaixa() - getArestaDaCaixa()/2.0;}

			inline double getArestaDaCaixa(){ return arestaDaCaixa;}
			inline void setArestaDaCaixa(double a){ if(a>0) this->arestaDaCaixa = a;}

			inline double getRaioEsfera(){ return esfera.raio;}
			inline void setRaioEsfera(double r){ if(r>0) esfera.raio = r;}

			inline Cor getCorEsfera(){ return esfera.cor;}
			inline void setCorEsfera(Cor r){ esfera.cor = r;}

			inline double getAlturaPrisma(){ return prismaTriangular.altura;}
			inline void setAlturaPrisma(double a){ if(a>0) prismaTriangular.altura = a;}

			inline double getBasePrisma(){ return prismaTriangular.base;}
			inline void setBasePrisma(double b){ if(b>0) prismaTriangular.base = b;}

			inline Cor getCorPrisma(){ return prismaTriangular.cor;}
			inline void setCorPrisma(Cor c){ prismaTriangular.cor = c;}


			inline double getRazaoAspectoDoPrismaTriangularTruncado(){ return prismaTriangularTruncado.razaoDeAspecto;}
			inline void setRazaoAspectoDoPrismaTriangularTruncado(double a){ 
				if(a>0 && a<=1) prismaTriangularTruncado.razaoDeAspecto = a;
				else throw new runtime_error("A razão de aspecto deve ser um valor no intervalo ]0,1]");
			}

			inline double getRazaoDeTruncamentoDoPrismaTriangularTruncado(){ return prismaTriangularTruncado.razaoDeTruncamento;}
			inline void setRazaoDeTruncamentoDoPrismaTriangularTruncado(double b){ 
				if(b>=0 && b <=0.5) prismaTriangularTruncado.razaoDeTruncamento = b;
				else throw new runtime_error("A razão de truncamento deve ser um valor no intervalo [0,0.5]");
			}

			inline double getL0DoPrismaTriangularTruncado(){ return prismaTriangularTruncado.L0;}
			inline void setL0DoPrismaTriangularTruncado(double L0){ if(L0>0) prismaTriangularTruncado.L0 = L0;}

			inline Cor getCorDoPrismaTriangularTruncado(){ return prismaTriangularTruncado.cor;}
			inline void setCorDoPrismaTriangularTruncado(Cor cor){ prismaTriangularTruncado.cor = cor;}

		};
	}

}
#endif