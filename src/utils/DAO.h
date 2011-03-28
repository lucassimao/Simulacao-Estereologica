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
		__int64 salvarDisco(__int64 planoDeCorte_id,Disco *d);
		__int64 salvarPoligono(__int64 planoDeCorte_id, Poligono *p);
		


	public:
		DAO( sqlite3 *db);
		// esses métodos retornam a chave primária. Retornam -1 se o registro ñ foi salvo
		void salvarInterceptosPorosos(__int64 planoDeCorte_id);
		__int64 salvarPlano(double y, double largura,Cor cor);
		__int64 salvarInterceptoDeArea(__int64 planoDeCorte_id,InterceptoDeArea *interceptoDeArea);
		__int64 salvarEstatisticas(int planoDeCorte_fk, double areaDosInterceptosColetados,
									double areaDoPlano, int qtdePontosInternos, int qtdePontosNaGrade, double volumeFaseSolida,double volumeFaseLigante);
		void salvarInterceptosLineares(__int64 interceptoDeArea_id,vector<InterceptoLinear*> interceptosLineares,InterceptoType tipoIntercepto);
		
};


#endif
