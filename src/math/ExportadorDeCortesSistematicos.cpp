#include "ExportadorDeCortesSistematicos.h"

#define PI 3.1415926

ExportadorDeCortesSistematicos::ExportadorDeCortesSistematicos(string &diretorio,const char* bancoDeDados, int qtdePlanos,SimulacaoCaixa *simulacao){
	 	
	this->diretorio = diretorio;
	this->qtdePlanos = qtdePlanos;
	this->simulacao=simulacao;

	int rc = sqlite3_open(bancoDeDados,&(this->db));
    if (rc)
    {
		throw runtime_error( sqlite3_errmsg(db) );
	    sqlite3_close(db);
	}
}

void ExportadorDeCortesSistematicos::salvarAreaDosPoligonos( int plano_pk, ofstream &outFile){
	sqlite3_stmt *poligonos_stmt = 0;
	ostringstream  poligonos_select;
	poligonos_select << "select rowid,area,perimetro,L0,razaoDeAspectoOriginaria,razaoDeTruncamentoOriginaria ";
	poligonos_select << "from poligonos where planoDeCorte_fk = ?1;";


	int res = sqlite3_prepare_v2(this->db,poligonos_select.str().c_str(),-1,&poligonos_stmt,NULL);
	
    if( res==SQLITE_OK && poligonos_stmt ){
		res = sqlite3_bind_int(poligonos_stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(poligonos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while(res != SQLITE_DONE){
			//int poligono_pk = sqlite3_column_int(poligonos_stmt,0);
			double area = sqlite3_column_double(poligonos_stmt,1);
			//double perimetro = sqlite3_column_double(poligonos_stmt,2);
			outFile << area << endl;
			res = sqlite3_step(poligonos_stmt);
		}	

		sqlite3_finalize(poligonos_stmt);
		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

}
void ExportadorDeCortesSistematicos::salvarAreaDosDiscos(int plano_pk, ofstream &outFile){
	sqlite3_stmt *discos_stmt = 0;
	ostringstream  discos_select;
	discos_select << "select raio from discos where planoDeCorte_fk = ?1;";


	int res = sqlite3_prepare_v2(this->db,discos_select.str().c_str(),-1,&discos_stmt,NULL);
	
    if( res==SQLITE_OK && discos_stmt ){
		res = sqlite3_bind_int(discos_stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(discos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			double raio = sqlite3_column_double(discos_stmt,0);
			//double perimetro = 2*PI *raio;
			double area = PI *raio*raio;
			outFile << area << endl;
			res = sqlite3_step(discos_stmt);
		}
		sqlite3_finalize(discos_stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

}

void ExportadorDeCortesSistematicos::salvarInterceptosLineares(int plano_pk, ofstream &outFile){
	sqlite3_stmt *interceptos_stmt = 0;
	ostringstream  interceptos_select;
	interceptos_select << "select tamanho from interceptosLineares where planoDeCorte_fk = ?1 order by tamanho;";


	int res = sqlite3_prepare_v2(this->db,interceptos_select.str().c_str(),-1,&interceptos_stmt,NULL);
	
    if( res==SQLITE_OK && interceptos_stmt ){
		res = sqlite3_bind_int(interceptos_stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(interceptos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			double tamanho = sqlite3_column_double(interceptos_stmt,0);
			outFile << tamanho << endl;

			res = sqlite3_step(interceptos_stmt);
		}
		sqlite3_finalize(interceptos_stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

}

void ExportadorDeCortesSistematicos::exportarPlano(int planoDeCorteID){
	ostringstream stream;

	locale ptBR(locale(),new WithComma);

	stream << this->diretorio << "/areas_plano_" << planoDeCorteID << ".csv"; 

	ofstream areasFile(stream.str().c_str(),std::ios::out);
	areasFile.imbue(ptBR);

	salvarAreaDosPoligonos(planoDeCorteID,areasFile);
	salvarAreaDosDiscos(planoDeCorteID,areasFile);
	areasFile.close();

	stream.str("");
	stream.clear();

	// agora salvando os interceptos lineares

	stream << this->diretorio << "/interceptosLineares_plano_" << planoDeCorteID << ".csv"; 
	ofstream interceptosLinearesFile(stream.str().c_str(),std::ios::out);
	interceptosLinearesFile.imbue(ptBR);

	salvarInterceptosLineares(planoDeCorteID,interceptosLinearesFile);
	interceptosLinearesFile.close();

	// exportando a quantidade de pontos também

	stream.str("");
	stream.clear();
	stream << this->diretorio << "/pontosInternos_plano_" << planoDeCorteID << ".csv"; 
	ofstream pontosInternosFile(stream.str().c_str(),std::ios::out);
	pontosInternosFile.imbue(ptBR);

	SalvarQtdeDePontosInternos(planoDeCorteID,pontosInternosFile);
	pontosInternosFile.close();
}

void ExportadorDeCortesSistematicos::SalvarQtdeDePontosInternos(int plano_pk, ofstream &outFile){
	sqlite3_stmt *stmt = 0;
	ostringstream  interceptos_select;
	interceptos_select << "select qtdeDePontosInternosAosInterceptos from estatisticas where planoDeCorte_fk = ?1;";


	int res = sqlite3_prepare_v2(this->db,interceptos_select.str().c_str(),-1,&stmt,NULL);
	
    if( res==SQLITE_OK && stmt ){
		res = sqlite3_bind_int(stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			int qtdeDePontosInternosAosInterceptos = sqlite3_column_int(stmt,0);
			outFile << qtdeDePontosInternosAosInterceptos << endl;

			res = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}

void ExportadorDeCortesSistematicos::exportarParaArquivo(){
	sqlite3_stmt *planoDeCorte_stmt = 0;
	ostringstream  planoDeCorte_select;
	planoDeCorte_select << "select rowid,altura from planoDeCorte;";


	int res = sqlite3_prepare_v2(this->db,planoDeCorte_select.str().c_str(),-1,&planoDeCorte_stmt,NULL);
	
    if( res==SQLITE_OK && planoDeCorte_stmt ){
		
		do{
			res = sqlite3_step(planoDeCorte_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			int planoPK = sqlite3_column_int(planoDeCorte_stmt,0);
			//double altura = sqlite3_column_double(planoDeCorte_stmt,1);
			exportarPlano(planoPK);
			res = sqlite3_step(planoDeCorte_stmt);
		}
		sqlite3_finalize(planoDeCorte_stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

}

void ExportadorDeCortesSistematicos::exportar(){
	simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy(this->qtdePlanos));
	DAO dao(this->db);
	
	NxActor* caixa = simulacao->getCaixa();
	NxActor* planoDeCorte = simulacao->getPlanoDeCorte()->getNxActor();
	
	Parametros *params = Parametros::getInstance();
	int qtdeLinhaNaGrade = params->getQtdeLinhasNaGrade()* params->getQtdePontosPorLinhaNaGrade();
	
	double volumeFaseSolida = simulacao->getVolumeFaseSolida();
	double volumeFaseLigante = simulacao->getVolumeFaseLigante();

	for(int i=0;i< this->qtdePlanos;++i){
		
		NxU32 qtdeAtores = simulacao->getQtdeObjetos();
		NxActor** atores = simulacao->getAtores();
		vector<Intercepto*> interceptos;
		simulacao->novoPlanoDeCorte();
		NxVec3 planoGlobalPosition = planoDeCorte->getGlobalPosition();

		ColetorDeInterceptosLinearesVisitor *visitor1 = new ColetorDeInterceptosLinearesVisitor(simulacao->getGrade());
		ColetorDePontosVisitor *visitor2 = new ColetorDePontosVisitor(simulacao->getGrade());
		ColetorDeAreasVisitor *visitor3 = new ColetorDeAreasVisitor(simulacao->getGrade());
		
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

	exportarParaArquivo();
	simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy());
	sqlite3_close(db);

}
		
