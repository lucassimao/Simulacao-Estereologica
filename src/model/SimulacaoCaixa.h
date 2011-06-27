#ifndef SimulacaoCaixa_H
#define SimulacaoCaixa_H

#include <NxPhysics.h>
#include "..\defs.h"
#include "..\utils\AbstractGeradorDeAlturaDoPlanoDeCorteStrategy.h"
#include "..\utils\GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h"
#include "..\utils\GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy.h"
#include "..\utils\MeshFactory.h"
#include "..\math\ColetorDeAreasVisitor.h"
#include "..\math\ColetorDePontosVisitor.h"
#include "..\utils\DAO.h"
#include "..\utils\DataBaseFactory.h"
#include "..\sqlite3\sqlite3.h"
#include "..\model\atores\PlanoDeCorte.h"
#include "grade\Grade.h"
#include "Parametros.h"
#include <QObject>

using namespace simulacao::math::mathVisitor;
using namespace simulacao::utils;
using namespace simulacao::model::atores;
using namespace simulacao::model::grade;

namespace simulacao{
	namespace model{

		class SimulacaoCaixa
		{

		private:
			
			bool simulacaoEmHardware;
			NxReal deltaTime; 
			Status status;
			const NxVec3 gravidade;
			NxScene *cena;
			NxPhysicsSDK *physicsSDK;
			const double fatorDeInterpenetracao;

			void initPhysicsSDK();

			inline NxPhysicsSDK* getSDK() const{
				return physicsSDK;
			}
			inline NxScene *getCena() const{
				return this->cena;
			}
			
			//

			NxActor *caixa;
			MeshFactory *meshFactory;
			PlanoDeCorte *atorPlanoDeCorte;
			AbstractGeradorDeAlturaDoPlanoDeCorteStrategy *alturaPlanoStrategy;
			bool exibirCaixa;
			bool _exibirPlanoDeCorte;
			bool exibirRetasTeste;
			bool exibirPontosTeste;
			DAO *dao;

		public:
			SimulacaoCaixa(double coeficienteDeInterpenetracao,double aceleracaoGravidade);

			void criarCaixa();
			void exibirPlanoDeCorte();
			void esconderPlanoDeCorte();
			double getVolumeFaseSolida();
			void adicionarEsferas(int qtde,Cor cor);
			void adicionarPrismas(double L0, int quantidade, Cor cor, double razaoDeAspecto, double razaoDeTruncamento);
			void novoPlanoDeCorte();
			void selecionarGraosInterceptados();
			void removerGraos();
			sqlite3 *executarCortesSistematicos(int);

			

			NxActor* getCaixa(){
				return this->caixa;
			}
			PlanoDeCorte *getPlanoDeCorte(){
				if (this->_exibirPlanoDeCorte)
					return this->atorPlanoDeCorte;
				else
					return NULL;
			}

			Grade * getGrade(){
				return getPlanoDeCorte()->getGrade();			
			}

			double getVolumeFaseLigante(){
				Parametros *params = Parametros::getInstance();
				return pow(params->getArestaDaCaixa(),3) - getVolumeFaseSolida();
			}
			
			double getVolumeDaCaixa(){
				Parametros *params = Parametros::getInstance();
				return pow(params->getArestaDaCaixa(),3);
			}
			
			void setGeradorDeAlturaDoPlanoStrategy(AbstractGeradorDeAlturaDoPlanoDeCorteStrategy* strategy){
				this->alturaPlanoStrategy = strategy;
			}

			bool getExibirRetasTeste(){ return this->exibirRetasTeste;}
			void setExibirRetasTeste(bool b){ this->exibirRetasTeste = b;}

			bool getExibirPontosTeste(){ return this->exibirPontosTeste;}
			void setExibirPontosTeste(bool b){ this->exibirPontosTeste = b;}

			//simulacao

			operator bool(){
				return ( cena!=NULL && status != PAUSADO);
			}
			void pararSimulacao() ;

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
			void desabilitarGravidade(){ 
				NxVec3 gravity;
				this->cena->getGravity(gravity);
				gravity.y = 0;
				this->cena->setGravity(gravity);
			}
			void habilitarGravidade(){ 
				NxVec3 gravity;
				this->cena->getGravity(gravity);
				gravity.y = -9.8;	
				this->cena->setGravity(gravity);
			}



			//end
			~SimulacaoCaixa(void);
		};

	}

}

#endif