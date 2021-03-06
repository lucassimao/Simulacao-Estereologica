#include <algorithm>
#include "..\model\Parametros.h"
#include "ExportadorParaArquivo.h"
#include "ProcessadorDeClassesDeIntercepto.h"

using namespace simulacao::exportador;
using simulacao::model::Parametros;

ExportadorParaArquivo::ExportadorParaArquivo(string &destino, sqlite3* db){
	this->destino = destino;
	this->db = db;
}

bool ExportadorParaArquivo::trabalharComPrismas(){
	sqlite3_stmt *contador_stmt = 0;
	const char *contador_select =  "select count(*) from poligonos;";

	int res = sqlite3_prepare_v2(this->db,contador_select,-1,&contador_stmt,NULL);
	assert( res==SQLITE_OK && contador_stmt );
	res = sqlite3_step(contador_stmt);
	assert(res == SQLITE_ROW);

	int qtde = sqlite3_column_int(contador_stmt,0);
	bool out = (qtde > 0);

	res = sqlite3_step(contador_stmt);
	assert(res == SQLITE_DONE);
	sqlite3_finalize(contador_stmt);
	return out;
}

void ExportadorParaArquivo::exportarPlanosDeCorte(){
	sqlite3_stmt *planoDeCorte_stmt = 0;
	ostringstream  planoDeCorte_select;
	planoDeCorte_select << "select rowid from planoDeCorte;";

	ostringstream arquivoQtdePontos;
	arquivoQtdePontos << this->destino << "/pontosInternos.csv"; 
	remove(arquivoQtdePontos.str().c_str());

	int res = sqlite3_prepare_v2(this->db,planoDeCorte_select.str().c_str(),-1,&planoDeCorte_stmt,NULL);

	if( res==SQLITE_OK && planoDeCorte_stmt ){

		do{
			res = sqlite3_step(planoDeCorte_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		while (res != SQLITE_DONE){
			int planoPK = sqlite3_column_int(planoDeCorte_stmt,0);
			exportarPlanoDeCorte(planoPK);
			res = sqlite3_step(planoDeCorte_stmt);
		}
		sqlite3_finalize(planoDeCorte_stmt);
	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

	exportarFracaoDePontos();
	exportarFracaoDeAreas();
	exportarFracaoLinear();
	salvarInterceptosDePoro();

	if (trabalharComPrismas()){
		exportarInterceptosMedioParaPrisma();
	}else{
		exportarInterceptosMedioParaEsfera();
	}


}


void ExportadorParaArquivo::exportarTabelasDeProbabilidade(int qtdeClassesDeIntercepto){
	ProcessadorDeClassesDeIntercepto processador(this->db);

	vector<ClasseDeGrao*> classesDeGrao = processador.getClassesDeGrao();
	map<TipoDeIntercepto,const char*> mapa;
	mapa[Linear] = "Comprimento_Linear";
	mapa[Area] = "Area";
	mapa[Perimetro] = "Perimetro";
	mapa[Poro] = "Poro";

	locale ptBR(locale(),new WithComma);

	map<TipoDeIntercepto,const char*>::const_iterator iterator = mapa.begin();
	while(iterator != mapa.end()){
		TipoDeIntercepto tipoDeIntercepto = (*iterator).first;
		const char *descricao = (*iterator).second;

		ostringstream nomeDoArqivo;
		nomeDoArqivo << this->destino << "/distribuicaoDeInterceptos_para_" << descricao << ".csv"; 
		ofstream arquivo(nomeDoArqivo.str().c_str(),std::ios::out);
		arquivo.imbue(ptBR);

		double menorIntercepto = processador.getMenorIntercepto(tipoDeIntercepto);
		double maiorIntercepto = processador.getMaiorIntercepto(tipoDeIntercepto);
		double deltaIntercepto = (maiorIntercepto - menorIntercepto)/qtdeClassesDeIntercepto;

		/*ostringstream cabecalho;
		if (tipoDeIntercepto != Poro ){
			for(int i=0; i<classesDeGrao.size() ;++i){ cabecalho << ";" << classesDeGrao[i]->getDiametroEquivalente(); }
		}else{
			cabecalho << ";" << "Quantidade";
		}
		arquivo << cabecalho.str() << ";Total" << std::endl;*/

		arquivo << ";Quantidade;Total" << std::endl;
		vector<vector<int>> tabelaDeDistribuicao = processador.gerarTabelaDeDistribuicaoDeInterceptos(tipoDeIntercepto,qtdeClassesDeIntercepto);

		map<int,int> quantidadePorClasseDeGrao;

		ostringstream tabela;
		for(int i=0; i < tabelaDeDistribuicao.size() ; ++i){

			double limInferior = menorIntercepto + i*deltaIntercepto;
			double limSuperior = limInferior + deltaIntercepto;

			tabela << limInferior << " |- " << limSuperior;

			int totalPorClasseDeIntercepto = 0; 
			for(int j=0;j< tabelaDeDistribuicao[i].size();++j){
				int quantidadeDeInterceptosNoIntervalo = tabelaDeDistribuicao[i][j];
				tabela << ";" << quantidadeDeInterceptosNoIntervalo;
				totalPorClasseDeIntercepto += quantidadeDeInterceptosNoIntervalo;

				quantidadePorClasseDeGrao[j] += quantidadeDeInterceptosNoIntervalo;
			}
			tabela << ";" << totalPorClasseDeIntercepto;
			tabela << std::endl;
		}
		arquivo << tabela.str();

		arquivo << "Totais";
		int total = 0;
		for(int j=0;j< quantidadePorClasseDeGrao.size();++j){
			total += quantidadePorClasseDeGrao[j];
			arquivo << ";" << quantidadePorClasseDeGrao[j];
		}
		arquivo << ";" << total << std::endl;
		arquivo.close();

		++iterator;
	}

}

void ExportadorParaArquivo::exportarPlanoDeCorte(int planoDeCorteID){

	locale ptBR(locale(),new WithComma);

	ostringstream arquivoAreasDoPlano;
	arquivoAreasDoPlano << this->destino << "/areas_plano_" << planoDeCorteID << ".csv"; 
	ofstream areasFile(arquivoAreasDoPlano.str().c_str(),std::ios::out);
	areasFile.imbue(ptBR);

	// agora salvando os interceptos de �rea
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

	// exportando a quantidade de pontos (exportado para um �nico arquivo )
	ostringstream arquivoQtdePontos;
	arquivoQtdePontos << this->destino << "/pontosInternos.csv"; 
	ofstream pontosInternosFile(arquivoQtdePontos.str().c_str(),std::ios_base::app);
	pontosInternosFile.imbue(ptBR);

	salvarQtdeDePontosInternos(planoDeCorteID,pontosInternosFile);
	pontosInternosFile.close();

	// agora salvando os interceptos porosos
	ostringstream arquivoInterceptosLivreCaminhoMedio;
	arquivoInterceptosLivreCaminhoMedio << this->destino << "/interceptosLivreCaminhoMedio_plano_" << planoDeCorteID << ".csv"; 
	ofstream interceptosLivreCaminhoMedioFile(arquivoInterceptosLivreCaminhoMedio.str().c_str(),std::ios::out);
	interceptosLivreCaminhoMedioFile.imbue(ptBR);

	salvarInterceptosDeLivreCaminhoMedio(planoDeCorteID,interceptosLivreCaminhoMedioFile);
	interceptosLivreCaminhoMedioFile.close();
}

void ExportadorParaArquivo::salvarInterceptosDeLivreCaminhoMedio(int plano_pk, ofstream &outFile){

	const char *interceptosPorosos_select = "select tamanho from interceptosPorosos where plano_fk=?;";
	sqlite3_stmt *interceptosPorosos_stmt = 0;
	int res = sqlite3_prepare_v2(this->db,interceptosPorosos_select,-1,&interceptosPorosos_stmt,NULL);

	if( res==SQLITE_OK && interceptosPorosos_stmt ){
		res = sqlite3_bind_int(interceptosPorosos_stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{ res = sqlite3_step(interceptosPorosos_stmt);}while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while(res != SQLITE_DONE){
			double tamanho = sqlite3_column_double(interceptosPorosos_stmt,0);

			outFile<< tamanho << std::endl;
			res = sqlite3_step(interceptosPorosos_stmt);
		}	

		sqlite3_finalize(interceptosPorosos_stmt);
	}
}

void ExportadorParaArquivo::salvarInterceptosDePoro(){
	locale ptBR(locale(),new WithComma);
	ostringstream fileName;
	fileName << this->destino << "/distribuicaoDeInterceptosPoro.csv"; 

	ofstream distribuicaoDeInterceptosPoro(fileName.str().c_str(),std::ios::out);
	distribuicaoDeInterceptosPoro.imbue(ptBR);

	double pesoGlobalDeInterceptosDeLivreCaminhoMedio = 0;
	double razaoTotal = 0;
	int quantidadeDePlanos=0;
	
	int quantidadeGlobalDeInterceptosDeLivreCaminhoMedio=0;
	double razaoTotalPeso=0;
	Parametros *params = Parametros::getInstance();
	double comprimentoDasRetasTeste = params->getArestaDaCaixa()*params->getParametrosDaGrade().qtdeLinhas;

	//
	double tamanhoTotalDosPorosNaSimulacao = 0.0;
	int qtdeTotalDosPorosNaSimulacao = 0;
	int qtdeTotalDeInterceptosLineares = 0;
	//

	const char *planoDeCorte_select = "select rowid from planoDeCorte;";
	sqlite3_stmt *planoDeCorte_stmt = 0;
	int res = sqlite3_prepare_v2(this->db,planoDeCorte_select,-1,&planoDeCorte_stmt,NULL);
	
	if( res==SQLITE_OK && planoDeCorte_stmt ){
		do{	res = sqlite3_step(planoDeCorte_stmt);}	while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		distribuicaoDeInterceptosPoro << "Plano;Intercepto de livre caminho m�dio"<<endl;
		
		//iterando sobre todos os planos 
		while (res != SQLITE_DONE){
			int planoDeCorte_id = sqlite3_column_int(planoDeCorte_stmt,0);	
			quantidadeDePlanos++;

			{
				// esse bloco encontra a quantidade e o tamanho total dos interceptos de livre caminho m�dio
				const char *interceptosPorosos_select = "select sum(peso),sum(tamanho),count(*) from interceptosPorosos where plano_fk=?;";
				sqlite3_stmt *interceptosPorosos_stmt = 0;
				int res = sqlite3_prepare_v2(this->db,interceptosPorosos_select,-1,&interceptosPorosos_stmt,NULL);

				if( res==SQLITE_OK && interceptosPorosos_stmt ){
					res = sqlite3_bind_int(interceptosPorosos_stmt,1,planoDeCorte_id);
					assert(res == SQLITE_OK);
					
					do{ res = sqlite3_step(interceptosPorosos_stmt);}while(res != SQLITE_ROW && res != SQLITE_DONE);
					
					while(res != SQLITE_DONE){
						double pesoDosInterceptosPorososNoPlano = sqlite3_column_double(interceptosPorosos_stmt,0);
						double tamanhoTotalDosPorosNoPlano = sqlite3_column_double(interceptosPorosos_stmt,1);
						int qtdeDeInterceptosPorososNoPlano = sqlite3_column_int(interceptosPorosos_stmt,2); 

						//
							tamanhoTotalDosPorosNaSimulacao += tamanhoTotalDosPorosNoPlano;
							qtdeTotalDosPorosNaSimulacao += tamanhoTotalDosPorosNoPlano;

						//

						pesoGlobalDeInterceptosDeLivreCaminhoMedio += pesoDosInterceptosPorososNoPlano;
						quantidadeGlobalDeInterceptosDeLivreCaminhoMedio += qtdeDeInterceptosPorososNoPlano;

						double comprimentoILC = getComprimentoILC(planoDeCorte_id);
						double comprimentoInterceptosLineares = getComprimentoInterceptosLineares(planoDeCorte_id);
						int qtdeInterceptosLinearesNoPlano = getQtdeInterceptosLineares(planoDeCorte_id);
						
						qtdeTotalDeInterceptosLineares += qtdeInterceptosLinearesNoPlano;

						distribuicaoDeInterceptosPoro << "Plano " <<planoDeCorte_id << ";";
						distribuicaoDeInterceptosPoro << comprimentoILC+comprimentoInterceptosLineares<< ";";
						distribuicaoDeInterceptosPoro << comprimentoDasRetasTeste<<std::endl;
						res = sqlite3_step(interceptosPorosos_stmt);
					}	

					sqlite3_finalize(interceptosPorosos_stmt);
				}
			}

			res = sqlite3_step(planoDeCorte_stmt);
		}
		sqlite3_finalize(planoDeCorte_stmt);
	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;


	distribuicaoDeInterceptosPoro << endl<< endl;
	distribuicaoDeInterceptosPoro << "ILCM 1;"<< tamanhoTotalDosPorosNaSimulacao/qtdeTotalDosPorosNaSimulacao <<endl; 
	distribuicaoDeInterceptosPoro << "ILCM 2;"<< tamanhoTotalDosPorosNaSimulacao/pesoGlobalDeInterceptosDeLivreCaminhoMedio <<endl; 

	double ilmt = getInterceptoLinearMedioTeorico();
	double volumeFaseSolida = getVolumeFaseSolida();
	double volumeTotalCaixa = pow(params->getArestaDaCaixa(),3);
	double fracaoFaseSolida = volumeFaseSolida/volumeTotalCaixa;

	double ilcmt3 = ilmt*( (1.0 - fracaoFaseSolida)/fracaoFaseSolida)*((double)qtdeTotalDeInterceptosLineares/qtdeTotalDosPorosNaSimulacao) ;
	distribuicaoDeInterceptosPoro << "ILCM 3;"<< ilcmt3 << endl;

	double ilcmt4 = ilmt*( (1.0 - fracaoFaseSolida)/fracaoFaseSolida )*((double)qtdeTotalDeInterceptosLineares/pesoGlobalDeInterceptosDeLivreCaminhoMedio) ;
	distribuicaoDeInterceptosPoro << "ILCM 4;"<< ilcmt4 << endl;

	distribuicaoDeInterceptosPoro.close();

}



double ExportadorParaArquivo::getComprimentoILC(int planoPK){

	const char *interceptosPorosos_select = "select sum(tamanho) from interceptosPorosos where plano_fk=?;";
	sqlite3_stmt *interceptosPorosos_stmt = 0;
	int res = sqlite3_prepare_v2(this->db,interceptosPorosos_select,-1,&interceptosPorosos_stmt,NULL);

	if( res==SQLITE_OK && interceptosPorosos_stmt ){
		res = sqlite3_bind_int(interceptosPorosos_stmt,1,planoPK);
		assert(res == SQLITE_OK);

		do{ res = sqlite3_step(interceptosPorosos_stmt);}while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		double comprimentoTotal = sqlite3_column_double(interceptosPorosos_stmt,0);

		res = sqlite3_step(interceptosPorosos_stmt);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(interceptosPorosos_stmt);
		return comprimentoTotal;
	}
	exit(EXIT_FAILURE);
	return -1;
}

int ExportadorParaArquivo::getQtdeInterceptosLineares(int planoPK){
	
	ProcessadorDeClassesDeIntercepto p(this->db);
	const char *select;

	if (p.getTipoDeGraoNaSimulacao() == Esferico){
		select = "select count(*) from discos d, interceptosLineares_discos id where d.planoDeCorte_fk=? and d.rowid=id.disco_fk;";
	}else{
		select = "select count(*) from poligonos p, interceptosLineares_poligonos ip where p.planoDeCorte_fk=? and p.rowid=ip.poligono_fk;";
	}
	sqlite3_stmt *interceptosPorosos_stmt = 0;
	int res = sqlite3_prepare_v2(this->db,select,-1,&interceptosPorosos_stmt,NULL);

	if( res==SQLITE_OK && interceptosPorosos_stmt ){
		res = sqlite3_bind_int(interceptosPorosos_stmt,1,planoPK);
		assert(res == SQLITE_OK);

		do{ res = sqlite3_step(interceptosPorosos_stmt);}while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		int qtde = sqlite3_column_int(interceptosPorosos_stmt,0);

		res = sqlite3_step(interceptosPorosos_stmt);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(interceptosPorosos_stmt);

		return qtde;
	}
	exit(EXIT_FAILURE);
	return -1;
}

double ExportadorParaArquivo::getComprimentoInterceptosLineares(int planoPK){
	
	ProcessadorDeClassesDeIntercepto p(this->db);
	const char *select;
	if (p.getTipoDeGraoNaSimulacao() == Esferico){
		select = "select sum(id.tamanho) from discos d, interceptosLineares_discos id where d.planoDeCorte_fk=? and d.rowid=id.disco_fk;";
	}else{
		select = "select sum(ip.tamanho) from poligonos p, interceptosLineares_poligonos ip where p.planoDeCorte_fk=? and p.rowid=ip.poligono_fk;";
	}
	sqlite3_stmt *interceptosPorosos_stmt = 0;
	int res = sqlite3_prepare_v2(this->db,select,-1,&interceptosPorosos_stmt,NULL);

	if( res==SQLITE_OK && interceptosPorosos_stmt ){
		res = sqlite3_bind_int(interceptosPorosos_stmt,1,planoPK);
		assert(res == SQLITE_OK);

		do{ res = sqlite3_step(interceptosPorosos_stmt);}while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		double comprimentoTotal = sqlite3_column_double(interceptosPorosos_stmt,0);

		res = sqlite3_step(interceptosPorosos_stmt);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(interceptosPorosos_stmt);

		return comprimentoTotal;
	}
	exit(EXIT_FAILURE);
	return -1;
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

void ExportadorParaArquivo::exportarFracaoLinear(){
	locale ptBR(locale(),new WithComma);
	string nomeArquivo = this->destino + "/fracaoLinear.csv"; 
	
	ofstream fracaoLinearFile(nomeArquivo.c_str(),std::ios::out);
	fracaoLinearFile.imbue(ptBR);

	fracaoLinearFile << "Plano; Comp. das linhas do plano; Comp. dos interceptos lineares; Fra��o linear" << endl;

	sqlite3_stmt *stmt = 0;
	const char *select = 0;
	if (trabalharComPrismas()){
		select = "select plano.rowid,sum(ip.tamanho) from planoDeCorte plano LEFT OUTER JOIN poligonos p "
			"ON plano.rowid=p.planoDeCorte_fk LEFT OUTER JOIN interceptosLineares_poligonos ip ON ip.poligono_fk=p.rowid "
			"group by plano.rowid order by plano.rowid;";
	}else{
		select = "select plano.rowid, sum(id.tamanho) from planoDeCorte plano LEFT OUTER JOIN discos d "
				 "ON plano.rowid=d.planoDeCorte_fk LEFT OUTER JOIN interceptosLineares_discos id ON id.disco_fk=d.rowid "
				 "group by plano.rowid order by plano.rowid;";		
	}
	int res = sqlite3_prepare_v2(this->db,select,-1,&stmt,NULL);
	if( res==SQLITE_OK && stmt ){

		do{
			res = sqlite3_step(stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		double comprimentoTotalDasLinhasDaGrade=0;
		double comprimentoTotalDosInterceptos = 0;
		const double comprimentoDasLinhasDaGrade = Parametros::getInstance()->getArestaDaCaixa()  *Parametros::getInstance()->getQtdeLinhasNaGrade() ;

		while (res != SQLITE_DONE){
			int plano = sqlite3_column_int(stmt,0);
			double comprimentoInterceptos = sqlite3_column_double(stmt,1);	
			if (comprimentoInterceptos == 0){
			
			}	

			double fracaoLinear = comprimentoInterceptos/comprimentoDasLinhasDaGrade;

			fracaoLinearFile <<"Plano " << plano << ";" << comprimentoDasLinhasDaGrade <<";"<< comprimentoInterceptos <<";"<<fracaoLinear<< endl;
			comprimentoTotalDasLinhasDaGrade+= comprimentoDasLinhasDaGrade;
			comprimentoTotalDosInterceptos += comprimentoInterceptos;

			res = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);	
		fracaoLinearFile << endl << endl;

		double fracaoLinearDaSimulacao = comprimentoTotalDosInterceptos/comprimentoTotalDasLinhasDaGrade;
		fracaoLinearFile << "Fra��o linear da simula��o;"<<fracaoLinearDaSimulacao << endl ;
	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

	fracaoLinearFile.close();
}

void ExportadorParaArquivo::exportarFracaoDeAreas(){
	locale ptBR(locale(),new WithComma);
	ostringstream arquivoFracaoDeAreas;
	arquivoFracaoDeAreas << this->destino << "/fracaoDeAreas.csv"; 

	ofstream fracaoDeAreasFile(arquivoFracaoDeAreas.str().c_str(),std::ios::out);
	fracaoDeAreasFile.imbue(ptBR);

	fracaoDeAreasFile << "Plano; �rea total da grade; �rea dos interceptos na grade do plano; Fra��o de �rea" << endl;

	sqlite3_stmt *stmt = 0;
	const char *select = "select planoDeCorte_fk,areaDoPlano, areaDosInterceptosColetados from estatisticas order by planoDeCorte_fk = ?1;";

	int res = sqlite3_prepare_v2(this->db,select,-1,&stmt,NULL);
	if( res==SQLITE_OK && stmt ){

		do{
			res = sqlite3_step(stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		double areaTotalDaGrade=0;
		double areaTotalDosInterceptos = 0;

		while (res != SQLITE_DONE){
			int plano = sqlite3_column_int(stmt,0);
			double areaPlano = sqlite3_column_double(stmt,1);
			double areaDosInterceptosColetados = sqlite3_column_double(stmt,2);
			double fracaoDeArea = areaDosInterceptosColetados/(double)areaPlano;

			fracaoDeAreasFile <<"Plano " << plano << ";" << areaPlano <<";"<< areaDosInterceptosColetados <<";"<<fracaoDeArea<< endl;
			areaTotalDaGrade+= areaPlano;
			areaTotalDosInterceptos += areaDosInterceptosColetados;

			res = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);	
		fracaoDeAreasFile << endl << endl;
		double fracaoDeAreaDaSimulacao = areaTotalDosInterceptos/(double)areaTotalDaGrade;
		fracaoDeAreasFile << "Fra��o de �rea da simula��o;"<<fracaoDeAreaDaSimulacao << endl ;
	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

	fracaoDeAreasFile.close();
}


void ExportadorParaArquivo::exportarFracaoDePontos(){
	locale ptBR(locale(),new WithComma);
	ostringstream arquivoFracaoDePontos;
	arquivoFracaoDePontos << this->destino << "/fracaoDePontos.csv"; 

	ofstream fracaoDePontosFile(arquivoFracaoDePontos.str().c_str(),std::ios::out);
	fracaoDePontosFile.imbue(ptBR);

	fracaoDePontosFile << "Plano; Qtde de Pontos na Grade; Qtde de pontos internos; Fra��o de pontos" << endl;

	sqlite3_stmt *stmt = 0;
	const char *select = "select planoDeCorte_fk,qtdePontosNaGrade, qtdeDePontosInternosAosInterceptos from estatisticas order by planoDeCorte_fk = ?1;";

	int res = sqlite3_prepare_v2(this->db,select,-1,&stmt,NULL);
	if( res==SQLITE_OK && stmt ){

		do{
			res = sqlite3_step(stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		int qtdeTotalDePontosNaGrade=0;
		int qtdeTotalDePontosInternosAosInterceptos = 0;

		while (res != SQLITE_DONE){
			int plano = sqlite3_column_int(stmt,0);
			int qtdePontosNaGrade = sqlite3_column_int(stmt,1);
			int qtdeDePontosInternosAosInterceptos = sqlite3_column_int(stmt,2);
			double fracaoDePontos = qtdeDePontosInternosAosInterceptos/(double)qtdePontosNaGrade;

			fracaoDePontosFile <<"Plano " << plano << ";" << qtdePontosNaGrade <<";"<< qtdeDePontosInternosAosInterceptos <<";"<<fracaoDePontos<< endl;
			qtdeTotalDePontosNaGrade+= qtdePontosNaGrade;
			qtdeTotalDePontosInternosAosInterceptos += qtdeDePontosInternosAosInterceptos;

			res = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);	
		fracaoDePontosFile << endl << endl;
		double fracaoDePontosDaSimulacao = qtdeTotalDePontosInternosAosInterceptos/(double)qtdeTotalDePontosNaGrade;
		fracaoDePontosFile << "Fra��o de Pontos da simula��o" <<";"<<fracaoDePontosDaSimulacao << endl ;
	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

	fracaoDePontosFile.close();
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
			outFile <<"Plano " << plano_pk << ";" <<  qtdeDePontosInternosAosInterceptos << endl;

			res = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);		
	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}

void ExportadorParaArquivo::exportarInterceptosMedioParaPrisma(){
	exportarInterceptoDeAreaMedioParaPrisma();
	exportarInterceptoDePerimetroMedioParaPrisma();
	exportarInterceptoLinearMedioParaPrisma();
}

void ExportadorParaArquivo::exportarInterceptosMedioParaEsfera(){
	exportarInterceptoDeAreaMedioParaEsfera();
	exportarInterceptoLinearMedioParaEsfera();
	exportarInterceptoDePerimetroMedioParaEsfera();
}

void ExportadorParaArquivo::exportarInterceptoLinearMedioParaPrisma(){
	locale ptBR(locale(),new WithComma);
	ostringstream fileName;
	fileName << this->destino << "/interceptoLinearMedioParaPrisma.csv"; 

	ofstream file(fileName.str().c_str(),std::ios::out);
	file.imbue(ptBR);
	file << "Plano;Altura do Plano;Intercepto linear m�dio" << std::endl;

	sqlite3_stmt *stmt = 0;
	ostringstream select;
	select << "select pla.rowid, pla.altura, avg(ip.tamanho) from poligonos po,planoDeCorte pla,interceptosLineares_poligonos ip";
	select << " where pla.rowid=po.planoDeCorte_fk and ip.poligono_fk=po.rowid group by pla.rowid order by pla.rowid;";

	int res = sqlite3_prepare_v2(this->db,select.str().c_str(),-1,&stmt,NULL);
	if( res==SQLITE_OK && stmt ){	

		do{
			res = sqlite3_step(stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		while(res != SQLITE_DONE){
			int idPlano = sqlite3_column_int(stmt,0);
			double alturaPlano =  sqlite3_column_double(stmt,1);
			double interceptoLinearMedio = sqlite3_column_double(stmt,2);
			file <<"Plano " << idPlano << ";" << alturaPlano << ";" << interceptoLinearMedio << std::endl;
			res = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);

		file << std::endl << std::endl;

		//coletando informa��es para o c�lculo de ILM
		sqlite3_stmt *stmt2 = 0;
		res = sqlite3_prepare_v2(this->db,"select avg(ip.tamanho) from interceptosLineares_poligonos ip;",-1,&stmt2,NULL);
		assert(res==SQLITE_OK && stmt2);
		sqlite3_step(stmt2);
		double ilm = sqlite3_column_double(stmt2,0);
		res = sqlite3_step(stmt2);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(stmt2);

		file << "ILM;" << ilm << std::endl;

		file << "ILMT;" << getInterceptoLinearMedioTeorico() << std::endl;

		file.close();

	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}

double ExportadorParaArquivo::getVolumeFaseSolida(){
	sqlite3_stmt *stmt = 0;
	const char *select = "select volumeFaseSolida from estatisticas";
	
	double volumeFaseSolida = 0;

	int res = sqlite3_prepare_v2(this->db,select,-1,&stmt,NULL);
	
    assert(res==SQLITE_OK && stmt );
			
	do{
		res = sqlite3_step(stmt);
	}
	while(res != SQLITE_ROW && res != SQLITE_DONE);
	
	volumeFaseSolida = sqlite3_column_double(stmt,0);
	res = sqlite3_step(stmt);

	//assert(res == SQLITE_DONE);

	sqlite3_finalize(stmt);
	return volumeFaseSolida;
}
double ExportadorParaArquivo::getInterceptoLinearMedioTeorico(){
		ProcessadorDeClassesDeIntercepto processador(this->db);

		
		double nominadorDaFormula=0;
		double denominadorDaFormula=0;

		if (processador.getTipoDeGraoNaSimulacao() == Prismatico){
			vector<ClasseDeGrao*> classes = processador.getClassesDeGraoPrismaticos();
			
			int qtdeDeClasses =  classes.size();
			for(int idx=0;idx<qtdeDeClasses;++idx){
				ClasseDeGraoPrismatico *classe = static_cast<ClasseDeGraoPrismatico*>(classes[idx]);
				double alpha = classe->razaoDeAspecto;
				double L = classe->L0;
				double beta = classe->razaoDeTruncamento;
				int n = classe->qtdeDeGraosDaClasse;

				nominadorDaFormula += n * ((sqrt(3.0)/4 )* alpha * (1 - 3*pow(beta,2)) * pow(L,3));
				denominadorDaFormula += n*(0.5)*(6*alpha*(1 - beta) + sqrt(3.0)*(1 - 3*pow(beta,2)))*pow(L,2);  
			}

			return 4*(nominadorDaFormula/denominadorDaFormula);
		}else{
			vector<ClasseDeGrao*> classes = processador.getClassesDeGraoEsfericos();

			int qtdeDeClasses =  classes.size();
			for(int idx=0;idx<qtdeDeClasses;++idx){
				ClasseDeGraoEsferico *classe = static_cast<ClasseDeGraoEsferico*>(classes[idx]);
				int n = classe->qtdeDeGraosDaClasse;
				double raio = classe->raio;


				nominadorDaFormula += n * (4.0/3.0) * raio;
				denominadorDaFormula += n;  
			}
			return nominadorDaFormula/denominadorDaFormula;
		}
}

void ExportadorParaArquivo::exportarInterceptoDePerimetroMedioParaPrisma(){
	locale ptBR(locale(),new WithComma);
	ostringstream fileName;
	fileName << this->destino << "/interceptoDePerimetroMedioParaPrisma.csv"; 

	ofstream file(fileName.str().c_str(),std::ios::out);
	file.imbue(ptBR);
	file << "Plano;Altura do Plano;Intercepto de per�metro m�dio" << std::endl;

	sqlite3_stmt *stmt = 0;
	ostringstream select;
	select << "select pla.rowid,pla.altura, avg(po.perimetro) from poligonos po,planoDeCorte pla where pla.rowid=po.planoDeCorte_fk group by pla.rowid order by pla.rowid;";

	int res = sqlite3_prepare_v2(this->db,select.str().c_str(),-1,&stmt,NULL);
	if( res==SQLITE_OK && stmt ){	

		do{
			res = sqlite3_step(stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		while(res != SQLITE_DONE){
			int idPlano = sqlite3_column_int(stmt,0);
			double alturaPlano =  sqlite3_column_double(stmt,1);
			double interceptoDePerimetroMedio = sqlite3_column_double(stmt,2);
			file <<"Plano "<<idPlano << ";"<< alturaPlano << ";" << interceptoDePerimetroMedio << std::endl;

			res = sqlite3_step(stmt);
		}	

		sqlite3_finalize(stmt);

		file << std::endl << std::endl;

		//coletando informa��es para o c�lculo do PM na simula��o
		sqlite3_stmt *stmt2 = 0;
		res = sqlite3_prepare_v2(this->db,"select avg(po.perimetro) from poligonos po;",-1,&stmt2,NULL);
		assert(res==SQLITE_OK && stmt2);
		sqlite3_step(stmt2);
		double pm = sqlite3_column_double(stmt2,0);
		res = sqlite3_step(stmt2);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(stmt2);

		file << "PM;" << pm << std::endl;

		// coletando informa��es para o c�lculo de PMT
		ProcessadorDeClassesDeIntercepto processador(this->db);
		vector<ClasseDeGrao*> classes = processador.getClassesDeGraoPrismaticos();

		int qtdeDeClasses =  classes.size();
		double nominadorDaFormula=0;
		double denominadorDaFormula=0;
		double pi = 3.14159265;

		//utilizando a f�rmula do PMT
		for(int idx=0;idx<qtdeDeClasses;++idx){
			ClasseDeGraoPrismatico *classe = static_cast<ClasseDeGraoPrismatico*>(classes[idx]);
			double alpha = classe->razaoDeAspecto;
			double L = classe->L0;
			double beta = classe->razaoDeTruncamento;
			int n = classe->qtdeDeGraosDaClasse;

			nominadorDaFormula += n * pow(L,2)*(6*alpha*(1 - beta) + sqrt(3.0)*(1 - 3*pow(beta,2)));
			denominadorDaFormula += n*L*(2*alpha + 3*(1 - beta));  

		}
		double Lp = (pi/2)*(nominadorDaFormula/denominadorDaFormula);
		file << "PMT;" << Lp << std::endl;

		file.close();

	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}

void ExportadorParaArquivo::exportarInterceptoDeAreaMedioParaPrisma(){
	locale ptBR(locale(),new WithComma);
	ostringstream fileName;
	fileName << this->destino << "/interceptoDeAreaMedioParaPrisma.csv"; 

	ofstream file(fileName.str().c_str(),std::ios::out);
	file.imbue(ptBR);
	file << "Plano;Altura do Plano;Intercepto de �rea m�dio" << std::endl;

	sqlite3_stmt *stmt = 0;
	ostringstream select;
	select << "select pla.rowid,pla.altura, avg(po.area) from poligonos po,planoDeCorte pla where pla.rowid=po.planoDeCorte_fk group by pla.rowid order by pla.rowid;";

	int res = sqlite3_prepare_v2(this->db,select.str().c_str(),-1,&stmt,NULL);
	if( res==SQLITE_OK && stmt ){	

		do{
			res = sqlite3_step(stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		while(res != SQLITE_DONE){
			int idPlano = sqlite3_column_int(stmt,0);
			double alturaPlano =  sqlite3_column_double(stmt,1);
			double interceptoDeAreaMedio = sqlite3_column_double(stmt,2);
			file <<"Plano "<<idPlano<< ";"<< alturaPlano<< ";" << interceptoDeAreaMedio << std::endl;

			res = sqlite3_step(stmt);
		}	

		sqlite3_finalize(stmt);

		file << std::endl << std::endl;

		//coletando informa��es para o c�lculo do IAM na simula��o
		sqlite3_stmt *stmt2 = 0;
		res = sqlite3_prepare_v2(this->db,"select avg(po.area) from poligonos po;",-1,&stmt2,NULL);
		assert(res==SQLITE_OK && stmt2);
		sqlite3_step(stmt2);
		double iam = sqlite3_column_double(stmt2,0);
		res = sqlite3_step(stmt2);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(stmt2);

		file << "IAM;" << iam << std::endl;

		// coletando informa��es para o c�lculo de IAMT
		ProcessadorDeClassesDeIntercepto processador(this->db);
		vector<ClasseDeGrao*> classes = processador.getClassesDeGraoPrismaticos();

		int qtdeDeClasses =  classes.size();
		int qtdeTotalDeGraos=0;
		double denominadorDaFormula =0;
		double nominadorDaFormula=0;

		//utilizando a f�rmula do IAMT
		for(int idx=0;idx<qtdeDeClasses;++idx){
			ClasseDeGraoPrismatico *classe = static_cast<ClasseDeGraoPrismatico*>(classes[idx]);
			double alpha = classe->razaoDeAspecto;
			double L = classe->L0;
			double beta = classe->razaoDeTruncamento;
			int n = classe->qtdeDeGraosDaClasse;

			nominadorDaFormula += n * pow(L,3)*(sqrt(3.0)* alpha * (1 - 3*pow(beta,2))); 
			denominadorDaFormula += n*L*(2*alpha+3*(1 - beta));  
		}
		double iamt = nominadorDaFormula/denominadorDaFormula;
		file << "IAMT;" << iamt << std::endl;


		file.close();

	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

}

void ExportadorParaArquivo::exportarInterceptoDePerimetroMedioParaEsfera(){
	locale ptBR(locale(),new WithComma);
	ostringstream fileName;
	fileName << this->destino << "/interceptoDePerimetroMedioParaEsferas.csv"; 

	ofstream file(fileName.str().c_str(),std::ios::out);
	file.imbue(ptBR);
	file << "Plano;Intercepto de per�metro m�dio" << std::endl;

	sqlite3_stmt *stmt = 0;
	ostringstream select;
	select << "select pla.rowid,pla.altura, avg(2*3.14159265*d.raio) from discos d,planoDeCorte pla where pla.rowid=d.planoDeCorte_fk group by pla.rowid order by pla.rowid;";

	int res = sqlite3_prepare_v2(this->db,select.str().c_str(),-1,&stmt,NULL);
	if( res==SQLITE_OK && stmt ){	

		do{
			res = sqlite3_step(stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		while(res != SQLITE_DONE){
			int plano = sqlite3_column_int(stmt,0);
			double alturaPlano =  sqlite3_column_double(stmt,1);
			double interceptoDePerimetroMedio = sqlite3_column_double(stmt,2);
			file << "Plano " << plano << "("<<alturaPlano<< ");" << interceptoDePerimetroMedio << std::endl;

			res = sqlite3_step(stmt);
		}	

		sqlite3_finalize(stmt);

		file << std::endl << std::endl;

		//coletando informa��es para o c�lculo do PM na simula��o
		sqlite3_stmt *stmt2 = 0;
		res = sqlite3_prepare_v2(this->db,"select avg(2*3.14159265*d.raio) from discos d;",-1,&stmt2,NULL);
		assert(res==SQLITE_OK && stmt2);
		sqlite3_step(stmt2);
		double pm = sqlite3_column_double(stmt2,0);
		res = sqlite3_step(stmt2);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(stmt2);

		file << "PM;" << pm << std::endl;


		// coletando informa��es para o c�lculo de PMT
		ProcessadorDeClassesDeIntercepto processador(this->db);
		vector<ClasseDeGrao*> classes = processador.getClassesDeGraoEsfericos();

		int qtdeDeClasses =  classes.size();
		int qtdeTotalDeGraos=0;
		double nominadorDaFormula=0;
		double pi = 3.14159265;

		//f�rmula do IPMT para esfera
		for(int idx=0;idx<qtdeDeClasses;++idx){
			ClasseDeGraoEsferico *classe = static_cast<ClasseDeGraoEsferico*>(classes[idx]);
			double raio = classe->raio;
			int qtdeDeGraosNaClasse = classe->qtdeDeGraosDaClasse;

			nominadorDaFormula += qtdeDeGraosNaClasse * (pow(pi,2)* raio)/2.0;

			qtdeTotalDeGraos += qtdeDeGraosNaClasse;
		}
		double Lp = nominadorDaFormula/qtdeTotalDeGraos;
		file << "PMT;" << Lp << std::endl;


		file.close();

	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}

void ExportadorParaArquivo::exportarInterceptoLinearMedioParaEsfera(){
	locale ptBR(locale(),new WithComma);
	ostringstream fileName;
	fileName << this->destino << "/interceptoLinearMedioParaEsferas.csv"; 

	ofstream file(fileName.str().c_str(),std::ios::out);
	file.imbue(ptBR);
	file << "Plano;Intercepto linear m�dio" << std::endl;

	sqlite3_stmt *stmt = 0;
	ostringstream select;
	select << "select pla.rowid,pla.altura, avg(id.tamanho) from discos d,planoDeCorte pla,interceptosLineares_discos id";
	select << " where pla.rowid=d.planoDeCorte_fk and id.disco_fk=d.rowid group by pla.rowid order by pla.rowid;";

	int res = sqlite3_prepare_v2(this->db,select.str().c_str(),-1,&stmt,NULL);
	if( res==SQLITE_OK && stmt ){	

		do{
			res = sqlite3_step(stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		while(res != SQLITE_DONE){
			int plano = sqlite3_column_int(stmt,0);
			double alturaPlano =  sqlite3_column_double(stmt,1);
			double interceptoLinearMedio = sqlite3_column_double(stmt,2);
			file << "Plano " << plano << "("<<alturaPlano<< ");" << interceptoLinearMedio << std::endl;

			res = sqlite3_step(stmt);
		}	

		sqlite3_finalize(stmt);

		file << std::endl << std::endl;

		//coletando informa��es para o c�lculo do ILM na simula��o
		sqlite3_stmt *stmt2 = 0;
		res = sqlite3_prepare_v2(this->db,"select avg(tamanho) from interceptosLineares_discos;",-1,&stmt2,NULL);
		assert(res==SQLITE_OK && stmt2);
		sqlite3_step(stmt2);
		double ilm = sqlite3_column_double(stmt2,0);
		res = sqlite3_step(stmt2);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(stmt2);

		file << "ILM;" << ilm << std::endl;


		// coletando informa��es para o c�lculo de ILMT
		ProcessadorDeClassesDeIntercepto processador(this->db);
		vector<ClasseDeGrao*> classes = processador.getClassesDeGraoEsfericos();

		int qtdeDeClasses =  classes.size();
		int qtdeTotalDeGraos=0;
		double nominadorDaFormula=0;

		//f�rmula do ILMT para esfera
		for(int idx=0;idx<qtdeDeClasses;++idx){
			ClasseDeGraoEsferico *classe = static_cast<ClasseDeGraoEsferico*>(classes[idx]);
			double raio = classe->raio;
			int qtdeDeGraosNaClasse = classe->qtdeDeGraosDaClasse;

			nominadorDaFormula += qtdeDeGraosNaClasse * 4/3.0 *raio;

			qtdeTotalDeGraos += qtdeDeGraosNaClasse;
		}
		double L3 = nominadorDaFormula/qtdeTotalDeGraos;
		file << "ILMT;" << L3 << std::endl;


		file.close();

	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}

void ExportadorParaArquivo::exportarInterceptoDeAreaMedioParaEsfera(){
	locale ptBR(locale(),new WithComma);
	ostringstream fileName;
	fileName << this->destino << "/interceptoDeAreaMedioParaEsfera.csv"; 

	ofstream file(fileName.str().c_str(),std::ios::out);
	file.imbue(ptBR);
	file << "Plano;Intercepto de �rea m�dio" << std::endl;

	sqlite3_stmt *stmt = 0;
	ostringstream select;
	select << "select pla.rowid,pla.altura, avg(3.14159265*d.raio*d.raio) from discos d,planoDeCorte pla where pla.rowid=d.planoDeCorte_fk group by pla.rowid order by pla.rowid;";

	int res = sqlite3_prepare_v2(this->db,select.str().c_str(),-1,&stmt,NULL);
	if( res==SQLITE_OK && stmt ){	

		do{
			res = sqlite3_step(stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);

		while(res != SQLITE_DONE){
			int plano = sqlite3_column_int(stmt,0);
			double alturaPlano =  sqlite3_column_double(stmt,1);
			double interceptoDeAreaMedio = sqlite3_column_double(stmt,2);
			file << "Plano " << plano << "("<<alturaPlano<< ");" << interceptoDeAreaMedio << std::endl;

			res = sqlite3_step(stmt);
		}	

		sqlite3_finalize(stmt);

		file << std::endl << std::endl;

		//coletando informa��es para o c�lculo do IAM na simula��o
		sqlite3_stmt *stmt2 = 0;
		res = sqlite3_prepare_v2(this->db,"select avg(3.14159265*d.raio*d.raio) from discos d;",-1,&stmt2,NULL);
		assert(res==SQLITE_OK && stmt2);
		sqlite3_step(stmt2);
		double iam = sqlite3_column_double(stmt2,0);
		res = sqlite3_step(stmt2);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(stmt2);

		file << "IAM;" << iam << std::endl;

		// coletando informa��es para o c�lculo de IAMT
		ProcessadorDeClassesDeIntercepto processador(this->db);
		vector<ClasseDeGrao*> classes = processador.getClassesDeGraoEsfericos();

		int qtdeDeClasses =  classes.size();
		int qtdeTotalDeGraos=0;
		double nominadorDaFormula=0;
		double pi = 3.14159265;

		//f�rmula do IAMT para esfera
		for(int idx=0;idx<qtdeDeClasses;++idx){
			ClasseDeGraoEsferico *classe = static_cast<ClasseDeGraoEsferico*>(classes[idx]);
			double raio = classe->raio;
			int qtdeDeGraosNaClasse = classe->qtdeDeGraosDaClasse;

			nominadorDaFormula += qtdeDeGraosNaClasse * 2/3.0 * pi* pow(raio,2);

			qtdeTotalDeGraos += qtdeDeGraosNaClasse;
		}
		double A = nominadorDaFormula/qtdeTotalDeGraos;
		file << "IAMT;" << A << std::endl;

		file.close();

	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}