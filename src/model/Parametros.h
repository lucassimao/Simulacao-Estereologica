#ifndef H_PARAMETROS
#define H_PARAMETROS

#include <stdexcept>
#include "..\utils\Observable.h"

using std::runtime_error;
using namespace simulacao::utils;


namespace simulacao{

	namespace model{
		typedef struct {
			double razaoDeAspecto;
			double razaoDeTruncamento;
			double L0;
			double altura;
		}PrismaTriangularTruncadoParams;

		typedef struct {
			double altura;
			double base;
		}PrismaTriangularParams;

		typedef struct {
			int qtdeLinhas;
			int qtdePontos;
			bool pontosAleatorios;
		}GradeParams;

		class Parametros : public Observable
		{
		private:
			Parametros();
			static Parametros* instance;

			double raioEsfera;
			double arestaCubo;
			double arestaDaCaixa;
			PrismaTriangularParams prismaTriangular;
			PrismaTriangularTruncadoParams prismaTriangularTruncado;
			GradeParams grade;
			


		public:
			static Parametros* getInstance(){
				if (!Parametros::instance)
					instance = new Parametros();

				return Parametros::instance;
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

			inline double getArestaDaCaixa(){ return arestaDaCaixa;}
			inline void setArestaDaCaixa(double a){ if(a>0) this->arestaDaCaixa = a;}

			inline double getRaioEsfera(){ return raioEsfera;}
			inline void setRaioEsfera(double r){ if(r>0) this->raioEsfera = r;}

			inline double getArestaCubo(){ return arestaCubo;}
			inline void setArestaCubo(double a){ if(a>0) this->arestaCubo = a;}

			inline double getAlturaPrisma(){ return prismaTriangular.altura;}
			inline void setAlturaPrisma(double a){ if(a>0) prismaTriangular.altura = a;}

			inline double getBasePrisma(){ return prismaTriangular.base;}
			inline void setBasePrisma(double b){ if(b>0) prismaTriangular.base = b;}


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


		};
	}

}
#endif