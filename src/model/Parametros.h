#ifndef H_PARAMETROS
#define H_PARAMETROS

namespace simulacao{

	namespace model{

		class Parametros
		{
		private:
			Parametros();
			static Parametros* instance;

			double raioEsfera;
			double arestaCubo;
			double alturaPrisma;
			double basePrisma;
			bool usarGraosAleatorios;

		public:
			static Parametros* getInstance(){
				if (!Parametros::instance)
					instance = new Parametros();

				return Parametros::instance;
			}

			double getRaioEsfera(){ return raioEsfera;}
			void setRaioEsfera(double r){ if(r>0) this->raioEsfera = r;}

			double getArestaCubo(){ return arestaCubo;}
			void setArestaCubo(double a){ if(a>0) this->arestaCubo = a;}

			double getAlturaPrisma(){ return alturaPrisma;}
			void setAlturaPrisma(double a){ if(a>0) this->alturaPrisma = a;}

			double getBasePrisma(){ return basePrisma;}
			void setBasePrisma(double b){ if(b>0) this->basePrisma = b;}

			bool isUsarGraosAleatorios() const { return this->usarGraosAleatorios;}
			void setUsarGraosAleatorios(bool b) { this->usarGraosAleatorios = b;}

		};
	}

}
#endif