#include <stdexcept>
#include "..\sqlite3\sqlite3.h"

#include "ExportadorDeCortesSistematicos.h"
#include "..\utils\GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h"
#include "..\utils\GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy.h"
#include "..\math\ColetorDeAreasVisitor.h"
#include "..\math\ColetorDePontosVisitor.h"
#include "..\utils\DAO.h"
#include "..\model\Parametros.h"

using namespace simulacao::math;
using namespace simulacao::model;
using namespace simulacao::math::mathVisitor;
using std::runtime_error;

ExportadorDeCortesSistematicos::ExportadorDeCortesSistematicos(const char* bancoDeDados, int qtdePlanos,SimulacaoCaixa *simulacao){
	 	
	this->qtdePlanos = qtdePlanos;
	this->simulacao=simulacao;

	int rc = sqlite3_open(bancoDeDados,&(this->db));
    if (rc)
    {
		throw runtime_error( sqlite3_errmsg(db) );
	    sqlite3_close(db);
	}
}

void ExportadorDeCortesSistematicos::exportar(){
	simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy(this->qtdePlanos));
	DAO dao(this->db);
	
	NxActor* caixa = simulacao->getCaixa();
	NxActor* planoDeCorte = simulacao->getPlanoDeCorte()->getNxActor();
	
	Parametros *params = Parametros::getInstance();
	int qtdeLinhaNaGrade = params->getQtdeLinhasNaGrade()* params->getQtdePontosPorLinhaNaGrade();
	
	double volumeFaseSolida = simulacao->getVolumeFaseSolida();

	for(int i=1;i<= this->qtdePlanos;++i){
		
		NxU32 qtdeAtores = simulacao->getQtdeObjetos();
		NxActor** atores = simulacao->getAtores();
		vector<Intercepto*> interceptos;
		simulacao->novoPlanoDeCorte();
		NxVec3 planoGlobalPosition = planoDeCorte->getGlobalPosition();

		ColetorDeAreasVisitor *visitor1 = new ColetorDeAreasVisitor(simulacao->getGrade());
		ColetorDePontosVisitor *visitor2 = new ColetorDePontosVisitor(simulacao->getGrade());
		
		__int64 planoID = dao.salvarPlano(planoGlobalPosition.y);

		while (qtdeAtores--)
		{
			NxActor* ator = *atores++;

			if (ator != caixa && ator!= planoDeCorte){
				Ator *a = (Ator *)ator->userData;					
				if (a->estaInterceptadoPeloPlano(planoGlobalPosition)){
					Intercepto *intercepto = a->getIntercepto(planoGlobalPosition);
					
					intercepto->accept(visitor1);
					intercepto->accept(visitor2);

					switch(intercepto->getType()){
					case Type_Disco:
						dao.salvarDisco(i,static_cast<Disco*>(intercepto));
						break;
					case Type_Poligono:
						dao.salvarPoligono(i,static_cast<Poligono*>(intercepto));
						break;
					}
				}
			}
		}
		dao.salvarEstatisticas(planoID,visitor1->getAreaTotalColetada(),400,
			visitor2->getQtdeDePontosInternosAInterceptosDeArea(),qtdeLinhaNaGrade,volumeFaseSolida);
	}

	simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy());

}
		
