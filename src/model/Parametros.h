#ifndef H_PARAMETROS
#define H_PARAMETROS

#include <stdexcept>

using std::runtime_error;

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

		class Parametros
		{
		private:
			Parametros();
			static Parametros* instance;

			double raioEsfera;
			double arestaCubo;
			bool usarGraosAleatorios;
			double arestaDaCaixa;
			PrismaTriangularParams prismaTriangular;
			PrismaTriangularTruncadoParams prismaTriangularTruncado;


		public:
			static Parametros* getInstance(){
				if (!Parametros::instance)
					instance = new Parametros();

				return Parametros::instance;
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

			inline bool isUsarGraosAleatorios() const { return this->usarGraosAleatorios;}
			inline void setUsarGraosAleatorios(bool b) { this->usarGraosAleatorios = b;}

			inline double getRazaoAspectoDoPrismaTriangularTruncado(){ return prismaTriangularTruncado.razaoDeAspecto;}
			inline void setRazaoAspectoDoPrismaTriangularTruncado(double a){ 
				if(a>0 && a<=1) prismaTriangularTruncado.razaoDeAspecto = a;
				else throw new runtime_error("A raz�o de aspecto deve ser um valor no intervalo ]0,1]");
			}

			inline double getRazaoDeTruncamentoDoPrismaTriangularTruncado(){ return prismaTriangularTruncado.razaoDeTruncamento;}
			inline void setRazaoDeTruncamentoDoPrismaTriangularTruncado(double b){ 
				if(b>=0 && b <=0.5) prismaTriangularTruncado.razaoDeTruncamento = b;
				else throw new runtime_error("A raz�o de truncamento deve ser um valor no intervalo [0,0.5]");
			}
			inline double getL0DoPrismaTriangularTruncado(){ return prismaTriangularTruncado.L0;}
			inline void setL0DoPrismaTriangularTruncado(double L0){ if(L0>0) prismaTriangularTruncado.L0 = L0;}


		};
	}

}
#endif