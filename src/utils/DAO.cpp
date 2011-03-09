#include <sstream>
#include <QDebug>
#include <string>
#include "..\sqlite3\sqlite3.h"
#include "..\model\interceptos\Disco.h"
#include "..\model\grade\Grade.h"
#include "..\model\grade\RetaDeTeste.h"

using namespace simulacao::model::grade;
using namespace simulacao::model::interceptos;

#include "DAO.h"

using namespace std;



DAO::DAO(sqlite3 *db):db(db){

}

void DAO::salvarInterceptosLineares(__int64 interceptoDeArea_id,vector<InterceptoLinear*> interceptosLineares,InterceptoType tipoIntercepto){	
	vector<InterceptoLinear*>::const_iterator iterator = interceptosLineares.begin();
	ostringstream  insert;
	switch(tipoIntercepto){
		case Type_Poligono:
			insert << "insert into interceptosLineares_poligonos('poligono_fk','x0','y0','z0','x1','y1','z1','tamanho') ";
			break;
		case Type_Disco:
			insert << "insert into interceptosLineares_discos('disco_fk','x0','y0','z0','x1','y1','z1','tamanho') ";
			break;			
	}
	insert << "values(?1,?2,?3,?4,?5,?6,?7,?8);";

	while(iterator != interceptosLineares.end()){
		InterceptoLinear *interceptoLinear  = *iterator;
		sqlite3_stmt *insert_stmt = 0;

		int res = sqlite3_prepare_v2(this->db,insert.str().c_str(),-1,&insert_stmt,NULL);
		if( res==SQLITE_OK && insert_stmt ){
			res = sqlite3_bind_int(insert_stmt,1,interceptoDeArea_id);
			assert(res == SQLITE_OK);

			res = sqlite3_bind_double(insert_stmt,2,interceptoLinear->p0.x);
			assert(res == SQLITE_OK);

			res = sqlite3_bind_double(insert_stmt,3,interceptoLinear->p0.y);
			assert(res == SQLITE_OK);

			res = sqlite3_bind_double(insert_stmt,4,interceptoLinear->p0.z);
			assert(res == SQLITE_OK);

			res = sqlite3_bind_double(insert_stmt,5,interceptoLinear->p1.x);
			assert(res == SQLITE_OK);

			res = sqlite3_bind_double(insert_stmt,6,interceptoLinear->p1.y);
			assert(res == SQLITE_OK);

			res = sqlite3_bind_double(insert_stmt,7,interceptoLinear->p1.z);
			assert(res == SQLITE_OK);

			res = sqlite3_bind_double(insert_stmt,8,interceptoLinear->tamanho());
			assert(res == SQLITE_OK);

			res = sqlite3_step(insert_stmt);
			assert(res == SQLITE_DONE);

			sqlite3_finalize(insert_stmt);	
		}
		else{
			qDebug() <<  sqlite3_errmsg(this->db)<<endl;
		}

		++iterator;
	}
}

__int64 DAO::salvarPlano(double y,double largura,Cor cor){
	char *errStr;
	ostringstream  insert;

	insert << "insert into planoDeCorte('altura','largura','r','g','b') values(" << y << "," << largura;	
	insert << "," << cor.r << "," << cor.g << "," << cor.b << ");";	
	
    int rc = sqlite3_exec(this->db,insert.str().c_str(), 0, 0, &errStr);

	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return -1;
    }
	// recupera o rowid do planoDeCorte recentemente inserido
	__int64 planoDeCorteID = sqlite3_last_insert_rowid(this->db);
	return planoDeCorteID;
}

__int64 DAO::salvarInterceptoDeArea(__int64 planoDeCorte_id,InterceptoDeArea *interceptoDeArea){
	switch(interceptoDeArea->getType()){
		case Type_Disco:
			return salvarDisco(planoDeCorte_id,static_cast<Disco*>(interceptoDeArea));
			break;
		case Type_Poligono:
			return salvarPoligono(planoDeCorte_id,static_cast<Poligono*>(interceptoDeArea));
			break;
	}
}

__int64 DAO::salvarDisco(__int64 planoDeCorte_id, Disco *d){
	char *errStr;
	ostringstream  insert;
	
	double corR = d->getCor().r, corG = d->getCor().g, corB = d->getCor().b;

	insert << "insert into discos('planoDeCorte_fk','raioOriginal','raio','xcentro',";
	insert << "'ycentro','zcentro','r','g','b') values(" << planoDeCorte_id << "," << d->raioDaEsferaDeOrigem;
	insert << ","<< d->raio <<","<< d->centro.x  << ","<< d->centro.y  << ","<< d->centro.z << ",";	
	insert << corR << "," << corG << "," << corB << ");";	
	
    int rc = sqlite3_exec(this->db,insert.str().c_str(), 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
		qDebug() << errStr;
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return -1;
    }
	__int64 discoID = sqlite3_last_insert_rowid(this->db);
	return discoID;
}

__int64 DAO::salvarPoligono(__int64 planoDeCorte_id, Poligono *p){
	char *errStr;
	ostringstream  insert;

	insert << "insert into poligonos('razaoDeAspectoOriginaria','razaoDeTruncamentoOriginaria',";
	insert << "'planoDeCorte_fk','L0','area','perimetro','r','g','b') values("<< p->razaoDeAspectoOriginal;
	insert << ","<< p->razaoDeTruncamentoOriginal <<","<< planoDeCorte_id <<",";
	insert << p->L0Original << "," << p->getArea() << "," << p->getPerimetro() << ",";	
	insert << p->getCor().r << "," << p->getCor().g << "," << p->getCor().b << ");";	
	
    int rc = sqlite3_exec(this->db,insert.str().c_str(), 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return false;
    }

	// retorna o ID do poligono recentemente salvo
	sqlite3_int64 poligonoID = sqlite3_last_insert_rowid(this->db);

	// inserindo os vertices do poligono tb
	list<Ponto> vertices = p->getVertices();
	list<Ponto>::const_iterator vertices_iterator = vertices.begin();
	int aux = 1;
	
	while(vertices_iterator!= vertices.end()){

		ostringstream  insert2;
		Ponto vertice = *vertices_iterator;
		
		insert2 << "insert into vertices_poligono('poligono_fk','x','y','z','posicao') ";
		insert2 << " values("<< poligonoID << "," << vertice.x << "," << vertice.y;
		insert2 << ","<< vertice.z <<"," <<aux << ");"; 

		int rc = sqlite3_exec(this->db,insert2.str().c_str(), 0, 0, &errStr);
		if ( rc!=SQLITE_OK )
		{
			//qDebug() << errStr;
			throw runtime_error(errStr);
			sqlite3_free(errStr);
			return -1;
		}
		++aux;
		++vertices_iterator;
	}
	

	return poligonoID;
}

__int64 DAO::salvarEstatisticas(int planoDeCorte_fk, double areaDosInterceptosColetados,
								double areaDoPlano, int qtdePontosInternos, int qtdePontosNaGrade, double volumeFaseSolida,double volumeFaseLigante){
	char *errStr;
	ostringstream  insert;

	insert << "insert into estatisticas('areaDoPlano','planoDeCorte_fk','areaDosInterceptosColetados',";	
	insert << "'qtdeDePontosInternosAosInterceptos','qtdePontosNaGrade','volumeFaseSolida','volumeFaseLigante') values(" << areaDoPlano;
	insert << "," << planoDeCorte_fk << "," << areaDosInterceptosColetados << ",";
	insert << qtdePontosInternos << "," << qtdePontosNaGrade <<"," << volumeFaseSolida <<","<<volumeFaseLigante<<  ");";
	
    int rc = sqlite3_exec(this->db,insert.str().c_str(), 0, 0, &errStr);

	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return -1;
    }
	// recupera o rowid da estatistica recentemente inserida
	__int64 estatisticaID = sqlite3_last_insert_rowid(this->db);
	return estatisticaID;
}