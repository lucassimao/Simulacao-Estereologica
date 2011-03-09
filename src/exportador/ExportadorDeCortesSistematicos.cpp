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
		dbFilename << "C:\\Users\\user\\Desktop\\" << timestamp << ".db";		
		bancoDeDadosFile = dbFilename.str();
	#endif

	sqlite3 *db = DataBaseFactory::getInstance()->criarBanco(bancoDeDadosFile.c_str());

    if (db)
    {
		return db;
	}else
	{
		qDebug() << sqlite3_errmsg(db);
		throw runtime_error( sqlite3_errmsg(db) );	
		sqlite3_close(db);
	}
}


sqlite3* ExportadorDeCortesSistematicos::exportar(){
	sqlite3 *db = newDb();
	DAO dao(db);

	Parametros *p = Parametros::getInstance();
	double h0 = p->getAlturaDaBaseDaCaixa();
	double h1 = p->getArestaDaCaixa() + p->getAlturaDaBaseDaCaixa();
	simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy(h0,h1,this->qtdePlanos));
	
	NxActor* caixa = simulacao->getCaixa();
	NxActor* planoDeCorte = simulacao->getPlanoDeCorte()->getNxActor();
	
	int qtdeDePontosNaGrade = p->getQtdeLinhasNaGrade()* p->getQtdePontosPorLinhaNaGrade();
	double larguraDoPlanoDeCorte = p->getLarguraDoPlanoDeCorte();
	
	for(int i=0;i< this->qtdePlanos;++i){
		simulacao->novoPlanoDeCorte();

		NxActor** atores = simulacao->getAtores();
		NxVec3 planoGlobalPosition = planoDeCorte->getGlobalPosition();
		NxU32 qtdeAtores = simulacao->getQtdeObjetos();
			
		ColetorDePontosVisitor *visitor2 = new ColetorDePontosVisitor(simulacao->getGrade());
		ColetorDeAreasVisitor *visitor3 = new ColetorDeAreasVisitor(simulacao->getGrade());
		
		__int64 planoID = dao.salvarPlano(planoGlobalPosition.y,larguraDoPlanoDeCorte,simulacao->getPlanoDeCorte()->cor);

		while (qtdeAtores--)
		{
			NxActor* ator = *atores++;

			if (ator != caixa && ator!= planoDeCorte){
				Ator *a = (Ator *)ator->userData;	

				if (a->estaInterceptadoPeloPlano(planoGlobalPosition)){
					InterceptoDeArea *intercepto = a->getIntercepto(planoGlobalPosition);
															
					intercepto->accept(visitor2);
					intercepto->accept(visitor3);
					
					__int64 interceptoID = dao.salvarInterceptoDeArea(planoID,intercepto);

					vector<InterceptoLinear*> interceptosLineares = intercepto->getInterceptosLineares(simulacao->getGrade());			
					dao.salvarInterceptosLineares(interceptoID,interceptosLineares,intercepto->getType());
				}
			}
		}

		double areaTotalColetada = visitor3->getAreaTotalColetada();
		double areaDoPlanoDeCorte = pow(larguraDoPlanoDeCorte,2.0);
		int qtdePontosInternosAInterceptosDeArea = visitor2->getQtdeDePontosInternosAInterceptosDeArea();
		double volumeFaseSolida = simulacao->getVolumeFaseSolida();
		double volumeFaseLigante = simulacao->getVolumeFaseLigante();

		dao.salvarEstatisticas(planoID,areaTotalColetada,areaDoPlanoDeCorte,qtdePontosInternosAInterceptosDeArea,
								qtdeDePontosNaGrade,volumeFaseSolida,volumeFaseLigante);
	}
	simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy());
	return db;

}
		
