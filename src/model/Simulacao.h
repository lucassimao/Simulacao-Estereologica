#ifndef H_SIMULACAO
#define H_SIMULACAO


#include "..\defs.h"
#include <NxPhysics.h>
#include <stdexcept>


using std::exception;

/**
* Definição abstrata de uma simulação
*
*/

namespace simulacao{

	namespace model{

		class Simulacao
		{
		protected:
			const static NxVec3 gravidadeDefault;

			bool simulacaoEmHardware;
			NxReal deltaTime; //intervalo de tempo p/ simulação
			Status status;
			NxVec3 *gravidade;
			NxScene *cena;
			NxPhysicsSDK *physicsSDK;

			virtual void initPhysicsSDK() throw (exception);

			inline NxPhysicsSDK* getSDK() const{
				return physicsSDK;
			}
			inline NxScene *getCena() const{
				return this->cena;
			}

		public:
			Simulacao(void);
			~Simulacao(void);

			operator bool(){
				return ( cena!=NULL && status != PAUSADO);
			}

			void GetPhysicsResults();
			void iniciarSimulacao();

			NxI64 getQtdeObjetos() const;

			void setGravidade(NxVec3 *);
			NxVec3 *getGravidade() const;

			void setPlanoDeCorte(NxVec3 *);
			NxVec3 *getPlanoDeCorte() const;


			NxActor **getAtores(){ return this->cena->getActors();};

			bool isSimulacaoEmHardware() const{
				return this->simulacaoEmHardware;
			}

			Status getStatus() const{
				return this->status;
			}

			void setStatus(Status status){
				this->status = status;
			}

			void releaseActor(NxActor &ator){
				this->cena->releaseActor(ator);
			}
			void desabilitarGravidade(){ this->gravidade->y = 0;}
			void habilitarGravidade(){ this->gravidade->y = Simulacao::gravidadeDefault.y ;exit(0);}



		};
	}

}
#endif