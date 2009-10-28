#ifndef H_SIMULACAO
#define H_SIMULACAO

#include <NxPhysics.h>
#include <stdexcept>
#include "../defs.h"


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
			bool modoDepuracao;
			NxReal deltaTime; //intervalo de tempo p/ simulação
			Status status;
			NxF32 zoom;
			NxI64 qtdeObjetos;
			NxVec3 *gravidade;
			NxVec3 *pontoDeVisualizacao;
			NxActor* atorCorrente;

			NxScene *cena;
			NxPhysicsSDK *physicsSDK;

			virtual void initPhysicsSDK() throw (exception);

		public:
			Simulacao(void);
			~Simulacao(void);

			operator bool(){
				return ( cena!=NULL && status != PAUSADO);
			}

			void GetPhysicsResults();
			void iniciarSimulacao();

			void setExibirPlanoDeCorte(bool);
			bool getExibirPlanoDeCorte() const;

			void setExibirRetasTeste(bool);
			bool getExibirRetasTeste() const;

			void setExibirPontosTeste(bool);
			bool getExibirPontosTeste() const;

			void setExibirCaixa(bool);
			bool getExibirCaixa() const;

			void setExibirTampaCaixa(bool);
			bool getExibirTampaCaixa() const;

			void setZoom(NxF32);
			NxF32 getZoom() const;

			void setQtdeObjetos(NxI64);
			NxI64 getQtdeObjetos() const;

			void setGravidade(NxVec3 *);
			NxVec3 *getGravidade() const;

			void setPlanoDeCorte(NxVec3 *);
			NxVec3 *getPlanoDeCorte() const;

			void setPontoDeVisualizacao(NxVec3 *);
			NxVec3 *getPontoDeVisualizacao() const;

			NxPhysicsSDK* getSDK() const{
				return physicsSDK;
			}
			NxScene *getCena() const{
				return this->cena;
			}

			bool isModoDepuracao(){
				return this->modoDepuracao;
			}
			void setModoDepuracao(bool b){ this->modoDepuracao = b; }

			NxActor *getAtorCorrente(){ return atorCorrente;};
			void setAtorCorrente(NxActor *actor){ this->atorCorrente = actor; }

			bool isSimulacaoEmHardware() const{
				return this->simulacaoEmHardware;
			}

			Status getStatus() const{
				return this->status;
			}

			void setStatus(Status status){
				this->status = status;
			}

			void desabilitarGravidade(){ this->gravidade->y = 0;}
			void habilitarGravidade(){ this->gravidade->y = Simulacao::gravidadeDefault.y ;exit(0);}



		};
	}

}
#endif