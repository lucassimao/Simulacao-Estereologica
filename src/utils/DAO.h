#ifndef DAO_H
#define DAO_H

#include "..\sqlite3\sqlite3.h"
#include "..\defs.h"
#include "..\model\interceptos\Disco.h"
#include "..\model\interceptos\Poligono.h"
#include "..\math\ColetorDeInterceptosLinearesVisitor.h"


using namespace simulacao::model::interceptos;
using namespace simulacao::math::mathVisitor;

class DAO{
	private:
		sqlite3 *db;
	public:
		DAO( sqlite3 *db);
		// esses métodos retornam a chave primária. Retornam -1 se o registro ñ foi salvo
		__int64 salvarPlano(double y, double largura,Cor cor);
		__int64 salvarDisco(int i,Disco *d);
		__int64 salvarPoligono(int planoDeCorte_id, Poligono *p);
		__int64 salvarEstatisticas(int planoDeCorte_fk, double areaDosInterceptosColetados,
									double areaDoPlano, int qtdePontosInternos, int qtdePontosNaGrade, double volumeFaseSolida,double volumeFaseLigante);
		__int64 salvarInterceptosLineares(int planoDeCorte_id,ColetorDeInterceptosLinearesVisitor *visitor);
		
};


#endif
