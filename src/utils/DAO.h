#ifndef DAO_H
#define DAO_H

#include "..\sqlite3\sqlite3.h"
#include "..\defs.h"
#include "..\model\interceptos\Disco.h"
#include "..\model\grade\Grade.h"
#include "..\model\grade\RetaDeTeste.h"
#include "..\model\interceptos\Poligono.h"
#include "..\math\ColetorDeInterceptosLinearesVisitor.h"
#include <map>
#include <vector>


using namespace std;
using namespace simulacao::model::grade;
using namespace simulacao::model::interceptos;
using namespace simulacao::math::mathVisitor;

class DAO{
	private:
		sqlite3 *db;
		struct {
			bool operator()(InterceptoLinear *i1, InterceptoLinear *i2) const{
				return i1->p0.x < i2->p0.x;
			}
		}InterceptoLinearCmp;	

		__int64 salvarDisco(__int64 planoDeCorte_id,Disco *d);
		__int64 salvarPoligono(__int64 planoDeCorte_id, Poligono *p);
		void salvarInterceptoPoroso(int planoDeCorte_id,double x0,double y0,double z0, double x1, double y1, double z1,double peso);
		map<double,vector<InterceptoLinear*>> getInterceptosLineares(__int64 planoDeCorte_id);

	public:
		DAO( sqlite3 *db);
		// esses métodos retornam a chave primária. Retornam -1 se o registro ñ foi salvo
		__int64 salvarPlano(double y, double largura,Cor cor);
		__int64 salvarInterceptoDeArea(__int64 planoDeCorte_id,InterceptoDeArea *interceptoDeArea);
		__int64 salvarEstatisticas(int planoDeCorte_fk, double areaDosInterceptosColetados,
									double areaDoPlano, int qtdePontosInternos, int qtdePontosNaGrade, double volumeFaseSolida,double volumeFaseLigante);
		void salvarInterceptosLineares(__int64 interceptoDeArea_id,vector<InterceptoLinear*> &interceptosLineares,InterceptoType tipoIntercepto);
		__int64 salvarPrisma(double razaoDeAspecto, double razaoDeTruncamento,double L0);

		sqlite3* getDB(){ return this->db;}
		void zerar();
		void salvarInterceptosPorosos(__int64 planoDeCorte_id,Grade *grade);

};


#endif
