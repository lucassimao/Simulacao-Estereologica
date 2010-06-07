#include <QtDebug>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include "..\sqlite3\sqlite3.h"

#include "ExportadorDeCortesSistematicos.h"
#include "..\utils\GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h"
#include "..\utils\GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy.h"
#include "..\math\ColetorDeAreasVisitor.h"
#include "..\math\ColetorDePontosVisitor.h"
#include "..\utils\DAO.h"
#include "..\model\Parametros.h"

using namespace simulacao::math;
using namespace simulacao::model;
using namespace simulacao::math::mathVisitor;

using namespace std;

ExportadorDeCortesSistematicos::ExportadorDeCortesSistematicos(const char *diretorio,const char* bancoDeDados, int qtdePlanos,SimulacaoCaixa *simulacao){
	 	
	this->qtdePlanos = qtdePlanos;
	this->simulacao=simulacao;

	int rc = sqlite3_open(bancoDeDados,&(this->db));
    if (rc)
    {
		throw runtime_error( sqlite3_errmsg(db) );
	    sqlite3_close(db);
	}
}

double ExportadorDeCortesSistematicos::calcularPerimetroPoligono(int poligono_pk,sqlite3 *db){
	sqlite3_stmt *vertices_stmt=0;
	ostringstream vertices_select;
	
	vertices_select << "select x,y,z from vertices_poligono where poligono_fk=?1 ";
	vertices_select << " order by posicao;";
	int res = sqlite3_prepare_v2(db,vertices_select.str().c_str(),-1,&vertices_stmt,NULL);

	if (res == SQLITE_OK && vertices_stmt){
		res = sqlite3_bind_int(vertices_stmt,1,poligono_pk);
		assert(res == SQLITE_OK);

		do{
			res = sqlite3_step(vertices_stmt);
		}
		while(res != SQLITE_ROW);
		list<Ponto> vertices;
		
		
		do{
			double x = sqlite3_column_double(vertices_stmt,0);
			double y = sqlite3_column_double(vertices_stmt,1);
			double z = sqlite3_column_double(vertices_stmt,2);
			Ponto p =  {x,y,z};
			vertices.push_back(p);
			res = sqlite3_step(vertices_stmt);
		}
		while(res == SQLITE_ROW);

		assert(vertices.size()>=3);
		Cor vermelho = VERMELHO;
		Poligono p(vermelho,vertices,0,0,0);
		sqlite3_finalize(vertices_stmt);
		
		return p.getPerimetro();
	}

	return 0;


}

int ExportadorDeCortesSistematicos::processarPoligonos(ExportadorDeCortesSistematicos *obj, int plano_pk){
	sqlite3_stmt *poligonos_stmt = 0;
	ostringstream  poligonos_select;
	poligonos_select << "select rowid,area,L0,razaoDeAspectoOriginaria,razaoDeTruncamentoOriginaria ";
	poligonos_select << "from poligonos where planoDeCorte_fk = ?1;";


	int res = sqlite3_prepare_v2(obj->db,poligonos_select.str().c_str(),-1,&poligonos_stmt,NULL);
	
    if( res==SQLITE_OK && poligonos_stmt ){
		res = sqlite3_bind_int(poligonos_stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(poligonos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while(res != SQLITE_DONE){
			int poligono_pk = sqlite3_column_int(poligonos_stmt,0);
			double area = sqlite3_column_double(poligonos_stmt,1);
			double perimetro = calcularPerimetroPoligono(poligono_pk,obj->db);
			res = sqlite3_step(poligonos_stmt);
		}	

		sqlite3_finalize(poligonos_stmt);
		
    }
	else
		qDebug() <<  sqlite3_errmsg(obj->db)<<endl;
	return 0;

}
int ExportadorDeCortesSistematicos::processarDiscos(ExportadorDeCortesSistematicos *obj, int plano_pk){
	sqlite3_stmt *discos_stmt = 0;
	ostringstream  discos_select;
	discos_select << "select raio from discos where planoDeCorte_fk = ?1;";


	int res = sqlite3_prepare_v2(obj->db,discos_select.str().c_str(),-1,&discos_stmt,NULL);
	
    if( res==SQLITE_OK && discos_stmt ){
		res = sqlite3_bind_int(discos_stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(discos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			double raio = sqlite3_column_double(discos_stmt,0);
			double perimetro = 3.14 * raio*raio;
			res = sqlite3_step(discos_stmt);
		}
		sqlite3_finalize(discos_stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(obj->db)<<endl;
	return 0;

}

int ExportadorDeCortesSistematicos::exportarPlanoCallback(void *instancia, int qtdeColunas, char **colunas, char **nomeColunas){
	int rowId = atoi(colunas[0]);
	char *altura = colunas[1];

	ExportadorDeCortesSistematicos *obj = static_cast<ExportadorDeCortesSistematicos *>(instancia);
	
	processarPoligonos(obj,rowId);
	processarDiscos(obj,rowId);

	return 0;
}


void ExportadorDeCortesSistematicos::exportarParaArquivo(){
  char *zErrMsg = 0;	
  int rc = sqlite3_exec(this->db, "select rowid,altura from planoDeCorte;", exportarPlanoCallback, this, &zErrMsg);

  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
}

void ExportadorDeCortesSistematicos::exportar(){
	simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy(this->qtdePlanos));
	DAO dao(this->db);
	
	NxActor* caixa = simulacao->getCaixa();
	NxActor* planoDeCorte = simulacao->getPlanoDeCorte()->getNxActor();
	
	Parametros *params = Parametros::getInstance();
	int qtdeLinhaNaGrade = params->getQtdeLinhasNaGrade()* params->getQtdePontosPorLinhaNaGrade();
	
	double volumeFaseSolida = simulacao->getVolumeFaseSolida();
	double volumeFaseLigante = simulacao->getVolumeFaseLigante();

	for(int i=1;i<= this->qtdePlanos;++i){
		
		NxU32 qtdeAtores = simulacao->getQtdeObjetos();
		NxActor** atores = simulacao->getAtores();
		vector<Intercepto*> interceptos;
		simulacao->novoPlanoDeCorte();
		NxVec3 planoGlobalPosition = planoDeCorte->getGlobalPosition();

		ColetorDeAreasVisitor *visitor1 = new ColetorDeAreasVisitor(simulacao->getGrade());
		ColetorDePontosVisitor *visitor2 = new ColetorDePontosVisitor(simulacao->getGrade());
		
		__int64 planoID = dao.salvarPlano(planoGlobalPosition.y);

		while (qtdeAtores--)
		{
			NxActor* ator = *atores++;

			if (ator != caixa && ator!= planoDeCorte){
				Ator *a = (Ator *)ator->userData;	

				if (a->estaInterceptadoPeloPlano(planoGlobalPosition)){

					Intercepto *intercepto = a->getIntercepto(planoGlobalPosition);
					
					//qDebug() << "1"<<endl;
					if (intercepto==NULL)
						continue;
					intercepto->accept(visitor1);
					//qDebug() << "2"<<endl;
					intercepto->accept(visitor2);
					//qDebug() << "3"<<endl;

					switch(intercepto->getType()){
					case Type_Disco:
						dao.salvarDisco(i,static_cast<Disco*>(intercepto));
						break;
					case Type_Poligono:
						dao.salvarPoligono(i,static_cast<Poligono*>(intercepto));
						break;
					}
				}
			}
		}
		dao.salvarEstatisticas(planoID,visitor1->getAreaTotalColetada(),400,
			visitor2->getQtdeDePontosInternosAInterceptosDeArea(),qtdeLinhaNaGrade,volumeFaseSolida,volumeFaseLigante);
	}

	exportarParaArquivo();
	simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy());

}
		
