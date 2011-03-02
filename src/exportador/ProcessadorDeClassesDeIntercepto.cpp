#include <algorithm>
#include <sstream>

#include "ProcessadorDeClassesDeIntercepto.h"

 
using namespace simulacao::exportador;


ProcessadorDeClassesDeIntercepto::ProcessadorDeClassesDeIntercepto(sqlite3 *db){
	this->db = db;
}

int ProcessadorDeClassesDeIntercepto::getQuantidadeDeInterceptosDeAreaNoIntervalo(double subClasseMinima,double subClasseMaxima,ClasseDeIntercepto classe){
	sqlite3_stmt *poligonos_stmt = 0;
	ostringstream  poligonos_select;
	poligonos_select << "select count(*) from poligonos where ";
	poligonos_select << "razaoDeAspectoOriginaria=?1 and razaoDeTruncamentoOriginaria=?2 and L0 = ?3 ";
	poligonos_select << " and area between ?4 and ?5;";


	int res = sqlite3_prepare_v2(this->db,poligonos_select.str().c_str(),-1,&poligonos_stmt,NULL);
	
    if( res==SQLITE_OK && poligonos_stmt ){
		res = sqlite3_bind_double(poligonos_stmt,1,classe.razaoDeAspecto);
		assert(res == SQLITE_OK);

		res = sqlite3_bind_double(poligonos_stmt,2,classe.razaoDeTruncamento);
		assert(res == SQLITE_OK);

		res = sqlite3_bind_double(poligonos_stmt,3,classe.L0);
		assert(res == SQLITE_OK);

		res = sqlite3_bind_double(poligonos_stmt,4,subClasseMinima);
		assert(res == SQLITE_OK);

		res = sqlite3_bind_double(poligonos_stmt,5,subClasseMaxima);
		assert(res == SQLITE_OK);

		do{
			res = sqlite3_step(poligonos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		int qtde = sqlite3_column_int(poligonos_stmt,0);			
		res = sqlite3_step(poligonos_stmt);
		assert(res == SQLITE_DONE);

		sqlite3_finalize(poligonos_stmt);	
		return qtde;
    }
	else{
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
		return -1;
	}

}

double ProcessadorDeClassesDeIntercepto::getMenorInterceptoDeArea(){
	sqlite3_stmt *poligonos_stmt = 0;
	const char *classes_select =  "select min(area) from poligonos;";

	int res = sqlite3_prepare_v2(this->db,classes_select,-1,&poligonos_stmt,NULL);
	
    if( res==SQLITE_OK && poligonos_stmt ){
		
		do{
			res = sqlite3_step(poligonos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		double menorArea = sqlite3_column_double(poligonos_stmt,0);
		res = sqlite3_step(poligonos_stmt);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(poligonos_stmt);
		return menorArea;
    }
	else{
		throw exception(sqlite3_errmsg(this->db));
	}

}
double ProcessadorDeClassesDeIntercepto::getMaiorInterceptoDeArea(){
	sqlite3_stmt *poligonos_stmt = 0;
	const char *classes_select =  "select max(area) from poligonos;";

	int res = sqlite3_prepare_v2(this->db,classes_select,-1,&poligonos_stmt,NULL);
	
    if( res==SQLITE_OK && poligonos_stmt ){
		
		do{
			res = sqlite3_step(poligonos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		double maiorArea = sqlite3_column_double(poligonos_stmt,0);
		res = sqlite3_step(poligonos_stmt);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(poligonos_stmt);
		return maiorArea;
    }
	else{
		throw exception(sqlite3_errmsg(this->db));
	}
}

vector<ClasseDeIntercepto> ProcessadorDeClassesDeIntercepto::getClassesDeIntercepto(){
	vector<ClasseDeIntercepto> vetor;

	sqlite3_stmt *poligonos_stmt = 0;
	const char *classes_select =  "select distinct razaoDeAspectoOriginaria, razaoDeTruncamentoOriginaria, L0 from poligonos;";

	int res = sqlite3_prepare_v2(this->db,classes_select,-1,&poligonos_stmt,NULL);
	
    if( res==SQLITE_OK && poligonos_stmt ){
		
		do{
			res = sqlite3_step(poligonos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			ClasseDeIntercepto classe;
			
			classe.razaoDeAspecto = sqlite3_column_double(poligonos_stmt,0);
			classe.razaoDeTruncamento = sqlite3_column_double(poligonos_stmt,1);
			classe.L0 = sqlite3_column_double(poligonos_stmt,2);
			
			vetor.push_back(classe);
			
			res = sqlite3_step(poligonos_stmt);
		}
		sqlite3_finalize(poligonos_stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;


	struct{
		bool operator()(ClasseDeIntercepto &c1, ClasseDeIntercepto &c2) const{
			return c1.getDiametroEquivalente() < c2.getDiametroEquivalente();
		}
	} ClasseDeInterceptoCmp;

	sort(vetor.begin(),vetor.end(),ClasseDeInterceptoCmp);

	return vetor;
}