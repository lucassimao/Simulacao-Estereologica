#include <algorithm>
#include "ExportadorParaArquivo.h"
#include "ProcessadorDeClassesDeIntercepto.h"

using namespace simulacao::exportador;

ExportadorParaArquivo::ExportadorParaArquivo(string &destino, sqlite3* db,int qtdeClassesDeIntercepto){
	this->destino = destino;
	this->db = db;
	this->qtdeClassesDeIntercepto = qtdeClassesDeIntercepto;
}

void ExportadorParaArquivo::exportar(){
	sqlite3_stmt *planoDeCorte_stmt = 0;
	ostringstream  planoDeCorte_select;
	planoDeCorte_select << "select rowid from planoDeCorte;";

	int res = sqlite3_prepare_v2(this->db,planoDeCorte_select.str().c_str(),-1,&planoDeCorte_stmt,NULL);
	
    if( res==SQLITE_OK && planoDeCorte_stmt ){
		
		do{
			res = sqlite3_step(planoDeCorte_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			int planoPK = sqlite3_column_int(planoDeCorte_stmt,0);
			exportarPlano(planoPK);
			res = sqlite3_step(planoDeCorte_stmt);
		}
		sqlite3_finalize(planoDeCorte_stmt);
		salvarTabelaDeProbabilidades();
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

}


void ExportadorParaArquivo::salvarTabelaDeProbabilidades(){
	ProcessadorDeClassesDeIntercepto processador(this->db);

	vector<ClasseDeGrao> classesDeGrao = processador.getClassesDeGrao();
	map<TipoDeIntercepto,const char*> mapa;
	mapa[Linear] = "Comprimento_Linear";
	mapa[Area] = "Area";
	mapa[Perimetro] = "Perimetro";

	locale ptBR(locale(),new WithComma);

	map<TipoDeIntercepto,const char*>::const_iterator iterator = mapa.begin();
	while(iterator != mapa.end()){
		TipoDeIntercepto tipoDeIntercepto = (*iterator).first;
		const char *descricao = (*iterator).second;

		ostringstream nomeDoArqivo;
		nomeDoArqivo << this->destino << "/tabelaProbabilidade_para_" << descricao << ".csv"; 
		ofstream arquivo(nomeDoArqivo.str().c_str(),std::ios::out);
		arquivo.imbue(ptBR);

		double menorIntercepto = processador.getMenorIntercepto(tipoDeIntercepto);
		double maiorIntercepto = processador.getMaiorIntercepto(tipoDeIntercepto);
		double deltaIntercepto = (maiorIntercepto - menorIntercepto)/this->qtdeClassesDeIntercepto;

		ostringstream cabecalho;
		for(int i=0; i<classesDeGrao.size() ;++i){ cabecalho << ";" << classesDeGrao[i].getDiametroEquivalente(); }
		arquivo << cabecalho.str() << std::endl;

		ostringstream tabela;
		for(int i=0; i < this->qtdeClassesDeIntercepto ; ++i){

			double limInferior = menorIntercepto + i*deltaIntercepto;
			double limSuperior = limInferior + deltaIntercepto;

			tabela << limInferior << " |- " << limSuperior;

			for(int j=0;j< classesDeGrao.size();++j){
				ClasseDeGrao classeDeGrao = classesDeGrao[j];
				tabela << ";" << processador.getQuantidadeDeInterceptosNoIntervalo(limInferior,limSuperior,classeDeGrao,tipoDeIntercepto);
			}
			tabela << std::endl;
		}
		arquivo << tabela.str();
		arquivo.close();

		++iterator;
	}

}

void ExportadorParaArquivo::exportarPlano(int planoDeCorteID){
	
	locale ptBR(locale(),new WithComma);

	ostringstream arquivoAreasDoPlano;
	arquivoAreasDoPlano << this->destino << "/areas_plano_" << planoDeCorteID << ".csv"; 
	ofstream areasFile(arquivoAreasDoPlano.str().c_str(),std::ios::out);
	areasFile.imbue(ptBR);

	// agora salvando os interceptos de área
	salvarAreaDosPoligonos(planoDeCorteID,areasFile);
	salvarAreaDosDiscos(planoDeCorteID,areasFile);
	areasFile.close();


	// agora salvando os interceptos lineares
	ostringstream arquivoInterceptosLineares;
	arquivoInterceptosLineares << this->destino << "/interceptosLineares_plano_" << planoDeCorteID << ".csv"; 
	ofstream interceptosLinearesFile(arquivoInterceptosLineares.str().c_str(),std::ios::out);
	interceptosLinearesFile.imbue(ptBR);

	salvarInterceptosLineares(planoDeCorteID,interceptosLinearesFile);
	interceptosLinearesFile.close();

	// exportando a quantidade de pontos
	ostringstream arquivoQtdePontos;
	arquivoQtdePontos << this->destino << "/pontosInternos_plano_" << planoDeCorteID << ".csv"; 
	ofstream pontosInternosFile(arquivoQtdePontos.str().c_str(),std::ios::out);
	pontosInternosFile.imbue(ptBR);

	salvarQtdeDePontosInternos(planoDeCorteID,pontosInternosFile);
	pontosInternosFile.close();

	salvarInterceptosDaFaseSolida(planoDeCorteID);
}

void ExportadorParaArquivo::salvarInterceptosDaFaseSolida(int plano_pk){
	locale ptBR(locale(),new WithComma);
	ostringstream fileName;
	fileName << this->destino << "/interceptosDaFaseSolida_plano_" << plano_pk << ".csv"; 

	ofstream interceptosDaFaseSolidaFile(fileName.str().c_str(),std::ios::out);
	interceptosDaFaseSolidaFile.imbue(ptBR);

	sqlite3_stmt *stmt = 0;
	ostringstream select;
	select << "select x0,y0,z0,x1,y1,z1 from interceptosLineares_discos where disco_fk in ";
	select << "(select rowid from discos where planoDeCorte_fk = ?1)";
	select << " union ";
	select << "select x0,y0,z0,x1,y1,z1 from interceptosLineares_poligonos where poligono_fk in ";
	select << "(select rowid from poligonos where planoDeCorte_fk = ?1);";


	int res = sqlite3_prepare_v2(this->db,select.str().c_str(),-1,&stmt,NULL);
	map<double,vector<InterceptoLinear*>> interceptosLineares;

    if( res==SQLITE_OK && stmt ){
		res = sqlite3_bind_int(stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while(res != SQLITE_DONE){
			Ponto p0,p1;
			p0.x = sqlite3_column_double(stmt,0);
			p0.y = sqlite3_column_double(stmt,1);
			p0.z = sqlite3_column_double(stmt,2);

			p1.x = sqlite3_column_double(stmt,3);
			p1.y = sqlite3_column_double(stmt,4);
			p1.z = sqlite3_column_double(stmt,5);			

			if (interceptosLineares.count(p0.z)==0){
				interceptosLineares[p0.z] = vector<InterceptoLinear*>();
			}
			interceptosLineares[p0.z].push_back(new InterceptoLinear(p0,p1));

			res = sqlite3_step(stmt);
		}	

		sqlite3_finalize(stmt);
		
		map<double,vector<InterceptoLinear*>>::const_iterator iterator = interceptosLineares.begin();

		while(iterator != interceptosLineares.end()){
			vector<InterceptoLinear*> vetor = (*iterator).second;
						
			if (vetor.size() > 1){
				sort(vetor.begin(),vetor.end(),InterceptoLinearCmp);				
				int iLinearAtual = 0;

				while(iLinearAtual+1 < vetor.size()){
					InterceptoLinear *iLinear = vetor[iLinearAtual];
					InterceptoLinear *iLinearSeguinte = vetor[iLinearAtual+1];

					if (iLinear->p1.x != iLinearSeguinte->p0.x){
						interceptosDaFaseSolidaFile << (iLinearSeguinte->p0.x - iLinear->p1.x) << std::endl;
					}
					++iLinearAtual;
				}
			}

			++iterator;
		}
		interceptosDaFaseSolidaFile.close();
		
		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
	
}

void ExportadorParaArquivo::salvarAreaDosPoligonos( int plano_pk, ofstream &outFile){
	sqlite3_stmt *poligonos_stmt = 0;
	ostringstream  poligonos_select;
	poligonos_select << "select area from poligonos where planoDeCorte_fk = ?1;";


	int res = sqlite3_prepare_v2(this->db,poligonos_select.str().c_str(),-1,&poligonos_stmt,NULL);
	
    if( res==SQLITE_OK && poligonos_stmt ){
		res = sqlite3_bind_int(poligonos_stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(poligonos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while(res != SQLITE_DONE){
			double area = sqlite3_column_double(poligonos_stmt,0);
			outFile << area << endl;
			res = sqlite3_step(poligonos_stmt);
		}	

		sqlite3_finalize(poligonos_stmt);
		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

}

void ExportadorParaArquivo::salvarAreaDosDiscos(int plano_pk, ofstream &outFile){
	sqlite3_stmt *discos_stmt = 0;
	ostringstream  discos_select;
	discos_select << "select raio from discos where planoDeCorte_fk = ?1;";


	int res = sqlite3_prepare_v2(this->db,discos_select.str().c_str(),-1,&discos_stmt,NULL);
	
    if( res==SQLITE_OK && discos_stmt ){
		res = sqlite3_bind_int(discos_stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(discos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			double raio = sqlite3_column_double(discos_stmt,0);
			//double perimetro = 2*PI *raio;
			double area = PI *raio*raio;
			outFile << area << endl;
			res = sqlite3_step(discos_stmt);
		}
		sqlite3_finalize(discos_stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

}


void ExportadorParaArquivo::salvarInterceptosLineares(int plano_pk, ofstream &outFile){
	sqlite3_stmt *interceptos_stmt = 0;
	ostringstream  interceptos_select;
	interceptos_select << "select tamanho from interceptosLineares_poligonos where poligono_fk in ";
	interceptos_select << "(select rowid from poligonos where planoDeCorte_fk = ?1) ";
	interceptos_select << " union all " ;
	interceptos_select << "select tamanho from interceptosLineares_discos where disco_fk in ";
	interceptos_select << "(select rowid from discos where planoDeCorte_fk = ?1) order by tamanho; ";

	int res = sqlite3_prepare_v2(this->db,interceptos_select.str().c_str(),-1,&interceptos_stmt,NULL);
	
    if( res==SQLITE_OK && interceptos_stmt ){
		res = sqlite3_bind_int(interceptos_stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(interceptos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			double tamanho = sqlite3_column_double(interceptos_stmt,0);
			outFile << tamanho << endl;

			res = sqlite3_step(interceptos_stmt);
		}
		sqlite3_finalize(interceptos_stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

}

void ExportadorParaArquivo::salvarQtdeDePontosInternos(int plano_pk, ofstream &outFile){
	sqlite3_stmt *stmt = 0;
	ostringstream  interceptos_select;
	interceptos_select << "select qtdeDePontosInternosAosInterceptos from estatisticas where planoDeCorte_fk = ?1;";


	int res = sqlite3_prepare_v2(this->db,interceptos_select.str().c_str(),-1,&stmt,NULL);
	
    if( res==SQLITE_OK && stmt ){
		res = sqlite3_bind_int(stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			int qtdeDePontosInternosAosInterceptos = sqlite3_column_int(stmt,0);
			outFile << qtdeDePontosInternosAosInterceptos << endl;

			res = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}