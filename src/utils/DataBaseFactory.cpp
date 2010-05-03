#include <stdexcept>
#include "DataBaseFactory.h"
#include "../sqlite3/sqlite3.h"

using std::exception;
using std::runtime_error;

DataBaseFactory* DataBaseFactory::instance = 0;

DataBaseFactory::DataBaseFactory(){

}

bool DataBaseFactory::criarBanco(const char* arquivo){
	sqlite3 *db;
    int rc;
    char *errStr;

    rc = sqlite3_open(arquivo,&db);
    if (rc)
    {
		throw runtime_error( sqlite3_errmsg(db) );
	    sqlite3_close(db);
        return false;
	}

    rc = sqlite3_exec(db, "create table planoDeCorte(id int PRIMARY KEY, altura double);", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
    }


    rc = sqlite3_exec(db, "create table retasDeTeste(id int PRIMARY KEY,x0 double,y0 double,z0 double,x1 double,y1 double,z1 double, planoDeCorte_fk int, FOREIGN KEY(planoDeCorte_fk) references planoDeCorte(id));", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
    }

    rc = sqlite3_exec(db, "create table pontosDeTeste(id int PRIMARY KEY,x double,y double,z double, retaDeTeste_fk int, FOREIGN KEY(retaDeTeste_fk) references retasDeTeste(id));", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
    }

	rc = sqlite3_exec(db, "create table poligonos(id int PRIMARY KEY, razaoDeAspectoOriginaria double, razaoDeTruncamentoOriginaria double, planoDeCorte_fk int, FOREIGN KEY(planoDeCorte_fk) references planoDeCorte(id));", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return false;
    }
	
	rc = sqlite3_exec(db,"create table discos(id int PRIMARY KEY,planoDeCorte_fk int,raioOriginal double,raio double,xcentro double, ycentro double, zcentro double,FOREIGN KEY(planoDeCorte_fk) references planoDeCorte(id));", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return false;
    }

	rc = sqlite3_exec(db,"create table vertices_poligono(id int PRIMARY KEY, poligono_fk int,x double,y double, z double,posicao int, FOREIGN KEY(poligono_fk) references poligonos(id));", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return false;
    }
	return true;
}