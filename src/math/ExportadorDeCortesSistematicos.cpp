#include <stdexcept>
#include "..\sqlite3\sqlite3.h"

#include "ExportadorDeCortesSistematicos.h"
#include "..\utils\GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h"
#include "..\utils\GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy.h"
#include "..\utils\DAO.h"

using namespace simulacao::math;
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

	for(int i=1;i<= this->qtdePlanos;++i){
		
		NxU32 qtdeAtores = simulacao->getQtdeObjetos();
		NxActor** atores = simulacao->getAtores();
		vector<Intercepto*> interceptos;
		simulacao->novoPlanoDeCorte();
		NxVec3 planoGlobalPosition = planoDeCorte->getGlobalPosition();
		
		dao.salvarPlano(planoGlobalPosition.y);

		while (qtdeAtores--)
		{
			NxActor* ator = *atores++;

			if (ator != caixa && ator!= planoDeCorte){
				Ator *a = (Ator *)ator->userData;					
				if (a->estaInterceptadoPeloPlano(planoGlobalPosition)){
					Intercepto *intercepto = a->getIntercepto(planoGlobalPosition);
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

	}

	simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy());

}
		
