#include "ExportadorParaArquivo.h"

using namespace simulacao::exportador;

ExportadorParaArquivo::ExportadorParaArquivo(string &destino, sqlite3* db){
	this->destino = destino;
	this->db = db;
}

void ExportadorParaArquivo::exportar(){
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
			exportarPlano(planoPK);
			res = sqlite3_step(planoDeCorte_stmt);
		}
		sqlite3_finalize(planoDeCorte_stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}

void ExportadorParaArquivo::exportarPlano(int planoDeCorteID){
	
	locale ptBR(locale(),new WithComma);

	ostringstream arquivoAreasDoPlano;
	arquivoAreasDoPlano << this->destino << "/areas_plano_" << planoDeCorteID << ".csv"; 
	ofstream areasFile(arquivoAreasDoPlano.str().c_str(),std::ios::out);
	areasFile.imbue(ptBR);

	// agora salvando os interceptos de �rea
	salvarAreaDosPoligonos(planoDeCorteID,areasFile);
	salvarAreaDosDiscos(planoDeCorteID,areasFile);
	areasFile.close();


	// agora salvando os interceptos lineares
	ostringstream arquivoInterceptosLineares;
	arquivoInterceptosLineares << this->destino << "/interceptosLineares_plano_" << planoDeCorteID << ".csv"; 
	ofstream interceptosLinearesFile(arquivoInterceptosLineares.str().c_str(),std::ios::out);
	interceptosLinearesFile.imbue(ptBR);

	salvarInterceptosLineares(planoDeCorteID,interceptosLinearesFile);
	interceptosLinearesFile.close();

	// exportando a quantidade de pontos
	ostringstream arquivoQtdePontos;
	arquivoQtdePontos << this->destino << "/pontosInternos_plano_" << planoDeCorteID << ".csv"; 
	ofstream pontosInternosFile(arquivoQtdePontos.str().c_str(),std::ios::out);
	pontosInternosFile.imbue(ptBR);

	salvarQtdeDePontosInternos(planoDeCorteID,pontosInternosFile);
	pontosInternosFile.close();
}

void ExportadorParaArquivo::salvarAreaDosPoligonos( int plano_pk, ofstream &outFile){
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

void ExportadorParaArquivo::salvarAreaDosDiscos(int plano_pk, ofstream &outFile){
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


void ExportadorParaArquivo::salvarInterceptosLineares(int plano_pk, ofstream &outFile){
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

void ExportadorParaArquivo::salvarQtdeDePontosInternos(int plano_pk, ofstream &outFile){
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