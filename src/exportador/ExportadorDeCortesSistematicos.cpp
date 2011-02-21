#include "ExportadorDeCortesSistematicos.h"

using namespace simulacao::exportador;

#define BANCO_EM_MEMORIA


ExportadorDeCortesSistematicos::ExportadorDeCortesSistematicos(int qtdePlanos,SimulacaoCaixa *simulacao){

	this->qtdePlanos = qtdePlanos;
	this->simulacao=simulacao;
}

sqlite3* ExportadorDeCortesSistematicos::newDb(){
	string bancoDeDadosFile;

	#ifdef BANCO_EM_MEMORIA
		bancoDeDadosFile = ":memory:";
	#else
		long timestamp = time(0);
		ostringstream dbFilename;
		dbFilename << diretorio << "/" << timestamp << ".db";		
		bancoDeDadosFile = dbFilename.str();
	#endif

	sqlite3 *db = DataBaseFactory::getInstance()->criarBanco(bancoDeDadosFile.c_str());

    if (db)
    {
		return db;
	}else
	{
		throw runtime_error( sqlite3_errmsg(db) );
	    sqlite3_close(db);
	}
}


sqlite3* ExportadorDeCortesSistematicos::exportar(){
	Parametros *p = Parametros::getInstance();
	double h0 = p->getAlturaDaBaseDaCaixa();
	double h1 = p->getArestaDaCaixa() + p->getAlturaDaBaseDaCaixa();
	simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy(h0,h1,this->qtdePlanos));

	sqlite3 *db = newDb();
	DAO dao(db);
	
	NxActor* caixa = simulacao->getCaixa();
	NxActor* planoDeCorte = simulacao->getPlanoDeCorte()->getNxActor();
	
	Parametros *params = Parametros::getInstance();
	int qtdeLinhaNaGrade = params->getQtdeLinhasNaGrade()* params->getQtdePontosPorLinhaNaGrade();
	double larguraDoPlanoDeCorte = params->getLarguraDoPlanoDeCorte();
	
	double volumeFaseSolida = simulacao->getVolumeFaseSolida();
	double volumeFaseLigante = simulacao->getVolumeFaseLigante();

	for(int i=0;i< this->qtdePlanos;++i){

		vector<Intercepto*> interceptos;

		simulacao->novoPlanoDeCorte();

		NxActor** atores = simulacao->getAtores();
		NxVec3 planoGlobalPosition = planoDeCorte->getGlobalPosition();
		NxU32 qtdeAtores = simulacao->getQtdeObjetos();
		
		ColetorDeInterceptosLinearesVisitor *visitor1 = new ColetorDeInterceptosLinearesVisitor(simulacao->getGrade());
		ColetorDePontosVisitor *visitor2 = new ColetorDePontosVisitor(simulacao->getGrade());
		ColetorDeAreasVisitor *visitor3 = new ColetorDeAreasVisitor(simulacao->getGrade());
		
		__int64 planoID = dao.salvarPlano(planoGlobalPosition.y,larguraDoPlanoDeCorte,simulacao->getPlanoDeCorte()->cor);

		while (qtdeAtores--)
		{
			NxActor* ator = *atores++;

			if (ator != caixa && ator!= planoDeCorte){
				Ator *a = (Ator *)ator->userData;	

				if (a->estaInterceptadoPeloPlano(planoGlobalPosition)){
					Intercepto *intercepto = a->getIntercepto(planoGlobalPosition);

					intercepto->accept(visitor1);
					intercepto->accept(visitor2);
					intercepto->accept(visitor3);

					switch(intercepto->getType()){
						case Type_Disco:
							dao.salvarDisco(planoID,static_cast<Disco*>(intercepto));
							break;
						case Type_Poligono:
							dao.salvarPoligono(planoID,static_cast<Poligono*>(intercepto));
							break;
					}
				}
			}
		}
		dao.salvarInterceptosLineares(planoID,visitor1);
		dao.salvarEstatisticas(planoID,visitor3->getAreaTotalColetada(),400,
			visitor2->getQtdeDePontosInternosAInterceptosDeArea(),qtdeLinhaNaGrade,volumeFaseSolida,volumeFaseLigante);
	}
	simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy());
	return db;

}
		
