#ifndef DAO_H
#define DAO_H

#include "..\sqlite3\sqlite3.h"
#include "..\model\interceptos\Disco.h"
#include "..\model\interceptos\Poligono.h"

using namespace simulacao::model::interceptos;

class DAO{
	private:
		sqlite3 *db;
	public:
		DAO( sqlite3 *db);
		bool salvarPlano(double y);
		bool salvarDisco(int i,Disco *d);
		bool salvarPoligono(int planoDeCorte_id, Poligono *p);
		
};


#endif
