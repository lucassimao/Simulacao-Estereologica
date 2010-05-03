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
		// esses métodos retornam a chave primária. Retornam -1 se o registro ñ foi salvo
		__int64 salvarPlano(double y);
		__int64 salvarDisco(int i,Disco *d);
		__int64 salvarPoligono(int planoDeCorte_id, Poligono *p);
		__int64 salvarEstatisticas(int planoDeCorte_fk, double areaDosInterceptosColetados,
									double areaDoPlano, int qtdePontosInternos, int qtdePontosNaGrade, double volumeFaseSolida,double volumeFaseLigante);
		
};


#endif
