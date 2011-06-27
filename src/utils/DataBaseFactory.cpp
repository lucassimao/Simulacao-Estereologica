#include <stdexcept>
#include <QtDebug>
#include "DataBaseFactory.h"
#include "../sqlite3/sqlite3.h"

using std::exception;
using std::runtime_error;

DataBaseFactory* DataBaseFactory::instance = 0;

DataBaseFactory::DataBaseFactory(){

}

sqlite3 * DataBaseFactory::criarBanco(const char* arquivo){
	sqlite3 *db;
    int rc;
    char *errStr;

    rc = sqlite3_open(arquivo,&db);
    if (rc)
    {
		qDebug() << sqlite3_errmsg(db);
		throw runtime_error( sqlite3_errmsg(db) );
	    sqlite3_close(db);
        return NULL;
	}

    rc = sqlite3_exec(db, "create table planoDeCorte(id int PRIMARY KEY, altura double,largura double,r float,g float,b float);", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return NULL;
    }

    rc = sqlite3_exec(db, "create table estatisticas(id int PRIMARY KEY, planoDeCorte_fk int, areaDoPlano double, areaDosInterceptosColetados double, qtdeDePontosInternosAosInterceptos int,qtdePontosNaGrade int , volumeFaseSolida double,volumeFaseLigante double, FOREIGN KEY(planoDeCorte_fk) references planoDeCorte(id));", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return NULL;
    }

	rc = sqlite3_exec(db, "create table prismas(id int PRIMARY KEY, razaoDeAspecto double, razaoDeTruncamento double, L0 double);", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return NULL;
    }


	rc = sqlite3_exec(db, "create table poligonos(id int PRIMARY KEY,atorID int,area double, razaoDeAspectoOriginaria double, razaoDeTruncamentoOriginaria double, planoDeCorte_fk int,L0 double,perimetro double,r double,g double,b double, FOREIGN KEY(planoDeCorte_fk) references planoDeCorte(id));", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return NULL;
    }
	
	rc = sqlite3_exec(db,"create table discos(id int PRIMARY KEY,atorID int,planoDeCorte_fk int,raioOriginal double,raio double,xcentro double, ycentro double, zcentro double,r double, g double, b double, FOREIGN KEY(planoDeCorte_fk) references planoDeCorte(id));", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return NULL;
    }

	rc = sqlite3_exec(db,"create table vertices_poligono(id int PRIMARY KEY, poligono_fk int,x double,y double, z double,posicao int, FOREIGN KEY(poligono_fk) references poligonos(id));", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return NULL;
    }

	rc = sqlite3_exec(db,"create table interceptosLineares_poligonos(id int PRIMARY KEY,poligono_fk int,x0 double,y0 double,z0 double,x1 double,y1 double,z1 double, tamanho double, FOREIGN KEY(poligono_fk) references poligonos(id));", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
		qDebug() << errStr;
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return NULL;
    }

	rc = sqlite3_exec(db,"create table interceptosLineares_discos(id int PRIMARY KEY,disco_fk int,x0 double,y0 double,z0 double,x1 double,y1 double,z1 double, tamanho double, FOREIGN KEY(disco_fk) references discos(id));", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return NULL;
    }

	rc = sqlite3_exec(db,"create table interceptosPorosos(plano_fk int,x0 double,y0 double,z0 double,x1 double,y1 double,z1 double, tamanho double);", 0, 0, &errStr);
	if ( rc!=SQLITE_OK )
    {
		qDebug() << errStr;
        throw runtime_error(errStr);
        sqlite3_free(errStr);
		return NULL;
    }

	return db;
}