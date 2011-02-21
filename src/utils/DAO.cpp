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

__int64 DAO::salvarInterceptosLineares(int planoDeCorte_id,ColetorDeInterceptosLinearesVisitor *visitor){
	char *errStr;
	ostringstream  insert;
	
	vector<double>::const_iterator iterator = visitor->interceptosLineares.begin();

	while(iterator!=visitor->interceptosLineares.end()){
		double interceptoLinear  = *iterator;

		insert << "insert into interceptosLineares('planoDeCorte_fk','tamanho')";
		insert << " values(" << planoDeCorte_id << "," << interceptoLinear << ");";
		++iterator;
	}

	int rc = sqlite3_exec(this->db,insert.str().c_str(), 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
	{
		qDebug() << errStr;
		throw runtime_error(errStr);
		sqlite3_free(errStr);
		return -1;
	}


	__int64 ultimoInterceptoID = sqlite3_last_insert_rowid(this->db);
	return ultimoInterceptoID;
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

__int64 DAO::salvarDisco(int planoDeCorte_id, Disco *d){
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