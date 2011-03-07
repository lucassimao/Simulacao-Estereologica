#include <algorithm>
#include <sstream>

#include "ProcessadorDeClassesDeIntercepto.h"

 
using namespace simulacao::exportador;


ProcessadorDeClassesDeIntercepto::ProcessadorDeClassesDeIntercepto(sqlite3 *db){
	this->db = db;
}

int ProcessadorDeClassesDeIntercepto::getQuantidadeDeInterceptosNoIntervalo(double subClasseMinima,double subClasseMaxima,ClasseDeGrao classe,TipoDeIntercepto tipoDeIntercepto){
	sqlite3_stmt *poligonos_stmt = 0;
	ostringstream  poligonos_select;
	poligonos_select << "select count(*) from poligonos where ";
	poligonos_select << "razaoDeAspectoOriginaria=?1 and razaoDeTruncamentoOriginaria=?2 and L0 = ?3 and ";
	switch(tipoDeIntercepto){
		case Area:
			poligonos_select << " area between ?4 and ?5;";
			break;
		case Perimetro:
			poligonos_select << " perimetro between ?4 and ?5;";
			break;
		case Linear:
			poligonos_select << " exists (select rowid from interceptosLineares_poligonos where poligono_fk = poligonos.rowid ";
			poligonos_select << " and tamanho between ?4 and ?5);";
	}


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

double ProcessadorDeClassesDeIntercepto::getMenorIntercepto(TipoDeIntercepto tipoDeIntercepto){
	sqlite3_stmt *poligonos_stmt = 0;
	const char *classes_select;

	switch(tipoDeIntercepto){
		case Area:
			classes_select = "select min(area) from poligonos;";
			break;
		case Perimetro:
			classes_select = "select min(perimetro) from poligonos;";
			break;
		case Linear:
			classes_select = "select min(tamanho) from interceptosLineares_poligonos;";
			break;
	}

	int res = sqlite3_prepare_v2(this->db,classes_select,-1,&poligonos_stmt,NULL);
	
    if( res==SQLITE_OK && poligonos_stmt ){
		
		do{
			res = sqlite3_step(poligonos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		double menorIntercepto = sqlite3_column_double(poligonos_stmt,0);
		res = sqlite3_step(poligonos_stmt);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(poligonos_stmt);
		return menorIntercepto;
    }
	else{
		throw exception(sqlite3_errmsg(this->db));
	}

}
double ProcessadorDeClassesDeIntercepto::getMaiorIntercepto(TipoDeIntercepto tipoDeIntercepto){
	sqlite3_stmt *poligonos_stmt = 0;
	const char *classes_select;
	switch(tipoDeIntercepto){
		case Area:
			classes_select = "select max(area) from poligonos;";
			break;
		case Perimetro:
			classes_select = "select max(perimetro) from poligonos;";
			break;
		case Linear:
			classes_select = "select max(tamanho) from interceptosLineares_poligonos;";
			break;
	}		

	int res = sqlite3_prepare_v2(this->db,classes_select,-1,&poligonos_stmt,NULL);
	
    if( res==SQLITE_OK && poligonos_stmt ){
		
		do{
			res = sqlite3_step(poligonos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		double maiorIntercepto = sqlite3_column_double(poligonos_stmt,0);
		res = sqlite3_step(poligonos_stmt);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(poligonos_stmt);
		return maiorIntercepto;
    }
	else{
		throw exception(sqlite3_errmsg(this->db));
	}
}

vector<ClasseDeGrao> ProcessadorDeClassesDeIntercepto::getClassesDeGrao(){
	vector<ClasseDeGrao> vetor;

	sqlite3_stmt *poligonos_stmt = 0;
	const char *classes_select =  "select distinct razaoDeAspectoOriginaria, razaoDeTruncamentoOriginaria, L0 from poligonos;";

	int res = sqlite3_prepare_v2(this->db,classes_select,-1,&poligonos_stmt,NULL);
	
    if( res==SQLITE_OK && poligonos_stmt ){
		
		do{
			res = sqlite3_step(poligonos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			ClasseDeGrao classe;
			
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
		bool operator()(ClasseDeGrao &c1, ClasseDeGrao &c2) const{
			return c1.getDiametroEquivalente() < c2.getDiametroEquivalente();
		}
	} ClasseDeGraoCmp;

	sort(vetor.begin(),vetor.end(),ClasseDeGraoCmp);

	return vetor;
}