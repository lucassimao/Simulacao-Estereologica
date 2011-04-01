#include <algorithm>
#include <sstream>

#include "ProcessadorDeClassesDeIntercepto.h"


using namespace simulacao::exportador;


ProcessadorDeClassesDeIntercepto::ProcessadorDeClassesDeIntercepto(sqlite3 *db){
	this->db = db;
	this->tipoDeGrao = __getTipoDeGraoNaSimulacao();
}

TipoDeGrao ProcessadorDeClassesDeIntercepto::getTipoDeGraoNaSimulacao(){
	return this->tipoDeGrao;
}

int ProcessadorDeClassesDeIntercepto::getQuantidadeDeInterceptosNoIntervalo(double subClasseMinima,double subClasseMaxima,ClasseDeGrao *classeDeGrao,TipoDeIntercepto tipoDeIntercepto){
	switch(this->tipoDeGrao){
		case Prismatico:
			{
				ClasseDeGraoPrismatico *classe = static_cast<ClasseDeGraoPrismatico*>(classeDeGrao);
				return getQuantidadeDeInterceptosPrismaticosNoIntervalo(subClasseMinima,subClasseMaxima,classe,tipoDeIntercepto);
			}
			break;
		case Esferico:
			{
				ClasseDeGraoEsferico *classe2 = static_cast<ClasseDeGraoEsferico*>(classeDeGrao);
				return getQuantidadeDeInterceptosEsfericosNoIntervalo(subClasseMinima,subClasseMaxima,classe2,tipoDeIntercepto);
			}
			break;
	}
}

int ProcessadorDeClassesDeIntercepto::getQuantidadeDeInterceptosPrismaticosNoIntervalo(double subClasseMinima,double subClasseMaxima,ClasseDeGraoPrismatico *classe,TipoDeIntercepto tipoDeIntercepto){
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
		res = sqlite3_bind_double(poligonos_stmt,1,classe->razaoDeAspecto);
		assert(res == SQLITE_OK);

		res = sqlite3_bind_double(poligonos_stmt,2,classe->razaoDeTruncamento);
		assert(res == SQLITE_OK);

		res = sqlite3_bind_double(poligonos_stmt,3,classe->L0);
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


int ProcessadorDeClassesDeIntercepto::getQuantidadeDeInterceptosEsfericosNoIntervalo(double subClasseMinima,double subClasseMaxima,ClasseDeGraoEsferico *classe,TipoDeIntercepto tipoDeIntercepto){
	sqlite3_stmt *discos_stmt = 0;
	ostringstream  discos_select;
	discos_select << "select count(*) from discos disco where raioOriginal=?1 and ";

	switch(tipoDeIntercepto){
		case Area:
			discos_select << " (3.14159265*raio*raio) between ?2 and ?3;";
			break;
		case Perimetro:
			discos_select << " (2*3.14159265*raio) between ?2 and ?3;";
			break;
		case Linear:
			discos_select << " exists (select rowid from interceptosLineares_discos where disco_fk = disco.rowid ";
			discos_select << " and tamanho between ?2 and ?3);";
	}

	int res = sqlite3_prepare_v2(this->db,discos_select.str().c_str(),-1,&discos_stmt,NULL);


	if( res==SQLITE_OK && discos_stmt ){
		res = sqlite3_bind_double(discos_stmt,1,classe->raio);
		assert(res == SQLITE_OK);

		res = sqlite3_bind_double(discos_stmt,2,subClasseMinima);
		assert(res == SQLITE_OK);

		res = sqlite3_bind_double(discos_stmt,3,subClasseMaxima);
		assert(res == SQLITE_OK);

		do{
			res = sqlite3_step(discos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		int qtde = sqlite3_column_int(discos_stmt,0);			
		res = sqlite3_step(discos_stmt);
		assert(res == SQLITE_DONE);

		sqlite3_finalize(discos_stmt);	
		return qtde;
	}
	else{
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
		return -1;
	}
}

double ProcessadorDeClassesDeIntercepto::getMenorInterceptoPrismatico(TipoDeIntercepto tipoDeIntercepto){
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


double ProcessadorDeClassesDeIntercepto::getMenorInterceptoEsferico(TipoDeIntercepto tipoDeIntercepto){
	sqlite3_stmt *discos_stmt = 0;
	const char *classes_select;

	switch(tipoDeIntercepto){
		case Area:
			classes_select = "select min(3.14159265*raio*raio) from discos;";
			break;
		case Perimetro:
			classes_select = "select min(2*3.14159265*raio) from discos;";
			break;
		case Linear:
			classes_select = "select min(tamanho) from interceptosLineares_discos;";
			break;
	}

	int res = sqlite3_prepare_v2(this->db,classes_select,-1,&discos_stmt,NULL);

	if( res==SQLITE_OK && discos_stmt ){

		do{
			res = sqlite3_step(discos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		double menorIntercepto = sqlite3_column_double(discos_stmt,0);

		res = sqlite3_step(discos_stmt);
		assert(res == SQLITE_DONE);

		sqlite3_finalize(discos_stmt);

		return menorIntercepto;
	}
	else{
		throw exception(sqlite3_errmsg(this->db));
	}
}


double ProcessadorDeClassesDeIntercepto::getMaiorInterceptoEsferico(TipoDeIntercepto tipoDeIntercepto){
	sqlite3_stmt *discos_stmt = 0;
	const char *classes_select;
	switch(tipoDeIntercepto){
		case Area:
			classes_select = "select max(3.14159265*raio*raio) from discos;";
			break;
		case Perimetro:
			classes_select = "select max(2*3.14159265*raio) from discos;";
			break;
		case Linear:
			classes_select = "select max(tamanho) from interceptosLineares_discos;";
			break;
	}		

	int res = sqlite3_prepare_v2(this->db,classes_select,-1,&discos_stmt,NULL);

	if( res==SQLITE_OK && discos_stmt ){

		do{
			res = sqlite3_step(discos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		double maiorIntercepto = sqlite3_column_double(discos_stmt,0);

		res = sqlite3_step(discos_stmt);
		assert(res == SQLITE_DONE);

		sqlite3_finalize(discos_stmt);
		return maiorIntercepto;
	}
	else{
		throw exception(sqlite3_errmsg(this->db));
	}
}


double ProcessadorDeClassesDeIntercepto::getMaiorInterceptoPrismatico(TipoDeIntercepto tipoDeIntercepto){
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


double ProcessadorDeClassesDeIntercepto::getMenorIntercepto(TipoDeIntercepto tipoDeIntercepto){
	switch(this->tipoDeGrao){
		case Esferico:
			return getMenorInterceptoEsferico(tipoDeIntercepto);
		case Prismatico:
			return getMenorInterceptoPrismatico(tipoDeIntercepto);
	}

}
double ProcessadorDeClassesDeIntercepto::getMaiorIntercepto(TipoDeIntercepto tipoDeIntercepto){
	switch(this->tipoDeGrao){
		case Esferico:
			return getMaiorInterceptoEsferico(tipoDeIntercepto);
		case Prismatico:
			return getMaiorInterceptoPrismatico(tipoDeIntercepto);
	}
}

vector<ClasseDeGrao*> ProcessadorDeClassesDeIntercepto::getClassesDeGrao(){
	switch(this->tipoDeGrao){
		case Esferico:
			return getClassesDeGraoEsfericos();
		case Prismatico:
			return getClassesDeGraoPrismaticos();
	}
}

vector<ClasseDeGrao*> ProcessadorDeClassesDeIntercepto::getClassesDeGraoPrismaticos(){
	vector<ClasseDeGrao*> vetor;

	sqlite3_stmt *poligonos_stmt = 0;
	const char *classes_select =  "select distinct razaoDeAspectoOriginaria, razaoDeTruncamentoOriginaria, L0 from poligonos;";

	int res = sqlite3_prepare_v2(this->db,classes_select,-1,&poligonos_stmt,NULL);

	if( res==SQLITE_OK && poligonos_stmt ){

		do{
			res = sqlite3_step(poligonos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		while (res != SQLITE_DONE){
			ClasseDeGraoPrismatico *classe = new ClasseDeGraoPrismatico;

			classe->razaoDeAspecto = sqlite3_column_double(poligonos_stmt,0);
			classe->razaoDeTruncamento = sqlite3_column_double(poligonos_stmt,1);
			classe->L0 = sqlite3_column_double(poligonos_stmt,2);

			vetor.push_back(classe);

			res = sqlite3_step(poligonos_stmt);
		}
		sqlite3_finalize(poligonos_stmt);		
	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

	sort(vetor.begin(),vetor.end(),ClasseDeGraoCmp);

	return vetor;
}

vector<ClasseDeGrao*> ProcessadorDeClassesDeIntercepto::getClassesDeGraoEsfericos(){
	vector<ClasseDeGrao*> vetor;

	sqlite3_stmt *discos_stmt = 0;
	const char *classes_select =  "select distinct raioOriginal from discos;";

	int res = sqlite3_prepare_v2(this->db,classes_select,-1,&discos_stmt,NULL);

	if( res==SQLITE_OK && discos_stmt ){

		do{
			res = sqlite3_step(discos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		while (res != SQLITE_DONE){
			ClasseDeGraoEsferico *classe = new ClasseDeGraoEsferico;

			classe->raio = sqlite3_column_double(discos_stmt,0);

			vetor.push_back(classe);

			res = sqlite3_step(discos_stmt);
		}
		sqlite3_finalize(discos_stmt);		
	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

	sort(vetor.begin(),vetor.end(),ClasseDeGraoCmp);

	return vetor;
}

TipoDeGrao ProcessadorDeClassesDeIntercepto::__getTipoDeGraoNaSimulacao(){

	sqlite3_stmt *contador_stmt = 0;
	const char *contador_select =  "select count(*) from poligonos;";

	int res = sqlite3_prepare_v2(this->db,contador_select,-1,&contador_stmt,NULL);

	assert( res==SQLITE_OK && contador_stmt );

	res = sqlite3_step(contador_stmt);
	assert(res == SQLITE_ROW);

	int qtde = sqlite3_column_double(contador_stmt,0);
	TipoDeGrao tipo;

	if (qtde == 0){
		tipo =  Esferico;
	}else{
		tipo=  Prismatico;
	}

	res = sqlite3_step(contador_stmt);
	assert(res == SQLITE_DONE);

	sqlite3_finalize(contador_stmt);
	return tipo;

}