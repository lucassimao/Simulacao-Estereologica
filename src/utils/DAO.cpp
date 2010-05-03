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

__int64 DAO::salvarPlano(double y){
	char *errStr;
	ostringstream  insert;

	insert << "insert into planoDeCorte('altura') values(" << y << ");";	
	
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

__int64 DAO::salvarDisco(int planoDeCorte_id, Disco *d){
	char *errStr;
	ostringstream  insert;
	
	insert << "insert into discos('planoDeCorte_fk','raioOriginal','raio','xcentro',";
	insert << "'ycentro','zcentro') values(" << planoDeCorte_id << "," << d->raioDaEsferaDeOrigem;
	insert << ","<< d->raio <<","<< d->centro.x  << ","<< d->centro.y  << ","<< d->centro.z << ");";	
	
    int rc = sqlite3_exec(this->db,insert.str().c_str(), 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
		//qDebug() << errStr;
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return -1;
    }
	__int64 discoID = sqlite3_last_insert_rowid(this->db);
	return discoID;
}

__int64 DAO::salvarPoligono(int planoDeCorte_id, Poligono *p){
	char *errStr;
	ostringstream  insert;

	insert << "insert into poligonos('razaoDeAspectoOriginaria','razaoDeTruncamentoOriginaria',";
	insert << "'planoDeCorte_fk','L0','area') values("<< p->razaoDeAspectoOriginal;
	insert << ","<< p->razaoDeTruncamentoOriginal <<","<< planoDeCorte_id <<","<< p->L0Original << "," << p->getArea() << ");";	
	
    int rc = sqlite3_exec(this->db,insert.str().c_str(), 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
		qDebug() << errStr;
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
								double areaDoPlano, int qtdePontosInternos, int qtdePontosNaGrade){
	char *errStr;
	ostringstream  insert;

	insert << "insert into estatisticas('areaDoPlano','planoDeCorte_fk','areaDosInterceptosColetados',";	
	insert << "'qtdeDePontosInternosAosInterceptos','qtdePontosNaGrade') values(" << areaDoPlano;
	insert << "," << planoDeCorte_fk << "," << areaDosInterceptosColetados << ",";
	insert << qtdePontosInternos << "," << qtdePontosNaGrade << ");";
	
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