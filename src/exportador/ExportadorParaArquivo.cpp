#include <algorithm>
#include "ExportadorParaArquivo.h"
#include "ProcessadorDeClassesDeIntercepto.h"

using namespace simulacao::exportador;

ExportadorParaArquivo::ExportadorParaArquivo(string &destino, sqlite3* db){
	this->destino = destino;
	this->db = db;
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

	// Verificando se deve exportar interceptos médios
	// para esfera ou prismas
	sqlite3_stmt *contador_stmt = 0;
	const char *contador_select =  "select count(*) from poligonos;";

	res = sqlite3_prepare_v2(this->db,contador_select,-1,&contador_stmt,NULL);
	assert( res==SQLITE_OK && contador_stmt );
	res = sqlite3_step(contador_stmt);
	assert(res == SQLITE_ROW);

	int qtde = sqlite3_column_double(contador_stmt,0);
	if (qtde == 0){
		exportarInterceptosMedioParaEsfera();
	}else{
		exportarInterceptosMedioParaPrisma();
	}

	res = sqlite3_step(contador_stmt);
	assert(res == SQLITE_DONE);
	sqlite3_finalize(contador_stmt);

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

		ostringstream cabecalho;
		if (tipoDeIntercepto != Poro ){
			for(int i=0; i<classesDeGrao.size() ;++i){ cabecalho << ";" << classesDeGrao[i]->getDiametroEquivalente(); }
		}else{
			cabecalho << ";" << "Quantidade";
		}
		arquivo << cabecalho.str() << ";Total" << std::endl;
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

	// exportando a quantidade de pontos (exportado para um único arquivo )
	ostringstream arquivoQtdePontos;
	arquivoQtdePontos << this->destino << "/pontosInternos.csv"; 
	ofstream pontosInternosFile(arquivoQtdePontos.str().c_str(),std::ios_base::app);
	pontosInternosFile.imbue(ptBR);

	salvarQtdeDePontosInternos(planoDeCorteID,pontosInternosFile);
	pontosInternosFile.close();

	salvarInterceptosDePoro(planoDeCorteID);
}

void ExportadorParaArquivo::salvarInterceptosDePoro(int plano_pk){
	locale ptBR(locale(),new WithComma);
	ostringstream fileName;
	fileName << this->destino << "/interceptosDePoro_plano_" << plano_pk << ".csv"; 

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
		struct {
			bool operator()(InterceptoLinear *i1, InterceptoLinear *i2) const{
				return i1->p0.x < i2->p0.x;
			}
		}InterceptoLinearCmp;
		map<double,vector<InterceptoLinear*>>::const_iterator iterator = interceptosLineares.begin();


		while(iterator != interceptosLineares.end()){
			vector<InterceptoLinear*> vetor = (*iterator).second;

			if (vetor.size() > 1){
				sort(vetor.begin(),vetor.end(),InterceptoLinearCmp);				
				int iLinearAtual = 0;

				while(iLinearAtual+1 < vetor.size()){
					InterceptoLinear *iLinear = vetor[iLinearAtual];
					InterceptoLinear *iLinearSeguinte = vetor[iLinearAtual+1];

					if ((iLinearSeguinte->p0.x - iLinear->p1.x)>0){
						interceptosDaFaseSolidaFile << iLinearSeguinte->p0.x - iLinear->p1.x << std::endl;
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

void ExportadorParaArquivo::exportarFracaoDeAreas(){
	locale ptBR(locale(),new WithComma);
	ostringstream arquivoFracaoDeAreas;
	arquivoFracaoDeAreas << this->destino << "/fracaoDeAreas.csv"; 

	ofstream fracaoDeAreasFile(arquivoFracaoDeAreas.str().c_str(),std::ios::out);
	fracaoDeAreasFile.imbue(ptBR);

	fracaoDeAreasFile << "Plano; Área total da grade; Área dos interceptos na grade do plano; Fração de área" << endl;

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
		fracaoDeAreasFile << "Fração de área da simulação;"<<fracaoDeAreaDaSimulacao << endl ;
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

	fracaoDePontosFile << "Plano; Qtde de Pontos na Grade; Qtde de pontos internos; Fração de pontos" << endl;

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
		fracaoDePontosFile << "Fração de Pontos da simulação" <<";"<<fracaoDePontosDaSimulacao << endl ;
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
	file << "Plano;Altura do Plano;Intercepto linear médio" << std::endl;

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

		//coletando informações para o cálculo de ILM
		sqlite3_stmt *stmt2 = 0;
		res = sqlite3_prepare_v2(this->db,"select avg(ip.tamanho) from interceptosLineares_poligonos ip;",-1,&stmt2,NULL);
		assert(res==SQLITE_OK && stmt2);
		sqlite3_step(stmt2);
		double ilm = sqlite3_column_double(stmt2,0);
		res = sqlite3_step(stmt2);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(stmt2);

		file << "ILM;" << ilm << std::endl;


		// coletando informações para o cálculo de ILMT
		ProcessadorDeClassesDeIntercepto processador(this->db);
		vector<ClasseDeGrao*> classes = processador.getClassesDeGraoPrismaticos();

		int qtdeDeClasses =  classes.size();
		int qtdeTotalDeGraos=0;
		double nominadorDaFormula=0;

		//utilizando a fórmula do ILMT
		for(int idx=0;idx<qtdeDeClasses;++idx){
			ClasseDeGraoPrismatico *classe = static_cast<ClasseDeGraoPrismatico*>(classes[idx]);
			double alpha = classe->razaoDeAspecto;
			double L = classe->L0;
			double beta = classe->razaoDeTruncamento;
			int n = classe->qtdeDeGraosDaClasse;
			nominadorDaFormula += n * (2 * sqrt(3.0) * alpha * (1 - 3*beta) * L)/( 6*alpha*(1 - beta) + sqrt(3.0)*(1 - 3*pow(beta,2) ) );

			qtdeTotalDeGraos += n;
		}
		double L3 = nominadorDaFormula/qtdeTotalDeGraos;
		file << "ILMT;" << L3 << std::endl;

		file.close();

	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}

void ExportadorParaArquivo::exportarInterceptoDePerimetroMedioParaPrisma(){
	locale ptBR(locale(),new WithComma);
	ostringstream fileName;
	fileName << this->destino << "/interceptoDePerimetroMedioParaPrisma.csv"; 

	ofstream file(fileName.str().c_str(),std::ios::out);
	file.imbue(ptBR);
	file << "Plano;Altura do Plano;Intercepto de perímetro médio" << std::endl;

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

		//coletando informações para o cálculo do PM na simulação
		sqlite3_stmt *stmt2 = 0;
		res = sqlite3_prepare_v2(this->db,"select avg(po.perimetro) from poligonos po;",-1,&stmt2,NULL);
		assert(res==SQLITE_OK && stmt2);
		sqlite3_step(stmt2);
		double pm = sqlite3_column_double(stmt2,0);
		res = sqlite3_step(stmt2);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(stmt2);

		file << "PM;" << pm << std::endl;

		// coletando informações para o cálculo de PMT
		ProcessadorDeClassesDeIntercepto processador(this->db);
		vector<ClasseDeGrao*> classes = processador.getClassesDeGraoPrismaticos();

		int qtdeDeClasses =  classes.size();
		int qtdeTotalDeGraos=0;
		double nominadorDaFormula=0;
		double pi = 3.14159265;

		//utilizando a fórmula do PMT
		for(int idx=0;idx<qtdeDeClasses;++idx){
			ClasseDeGraoPrismatico *classe = static_cast<ClasseDeGraoPrismatico*>(classes[idx]);
			double alpha = classe->razaoDeAspecto;
			double L = classe->L0;
			double beta = classe->razaoDeTruncamento;
			int n = classe->qtdeDeGraosDaClasse;

			nominadorDaFormula += n * pi*L*( 6*alpha*(1 - beta) + sqrt(3.0)*(1 - 3*pow(beta,2) ) )/(2 * (2*alpha + 3*(1-beta) ) );

			qtdeTotalDeGraos += n;
		}
		double Lp = nominadorDaFormula/qtdeTotalDeGraos;
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
	file << "Plano;Altura do Plano;Intercepto de área médio" << std::endl;

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

		//coletando informações para o cálculo do IAM na simulação
		sqlite3_stmt *stmt2 = 0;
		res = sqlite3_prepare_v2(this->db,"select avg(po.area) from poligonos po;",-1,&stmt2,NULL);
		assert(res==SQLITE_OK && stmt2);
		sqlite3_step(stmt2);
		double iam = sqlite3_column_double(stmt2,0);
		res = sqlite3_step(stmt2);
		assert(res == SQLITE_DONE);
		sqlite3_finalize(stmt2);

		file << "IAM;" << iam << std::endl;

		// coletando informações para o cálculo de IAMT
		ProcessadorDeClassesDeIntercepto processador(this->db);
		vector<ClasseDeGrao*> classes = processador.getClassesDeGraoPrismaticos();

		int qtdeDeClasses =  classes.size();
		int qtdeTotalDeGraos=0;
		double nominadorDaFormula=0;

		//utilizando a fórmula do IAMT
		for(int idx=0;idx<qtdeDeClasses;++idx){
			ClasseDeGraoPrismatico *classe = static_cast<ClasseDeGraoPrismatico*>(classes[idx]);
			double alpha = classe->razaoDeAspecto;
			double L = classe->L0;
			double beta = classe->razaoDeTruncamento;
			int n = classe->qtdeDeGraosDaClasse;
			nominadorDaFormula += n * ( sqrt(3.0) * alpha* (1 - 3*pow(beta,2)) * pow(L,2) )/(2*alpha + 3*(1-beta) );

			qtdeTotalDeGraos += n;
		}
		double iamt = nominadorDaFormula/qtdeTotalDeGraos;
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
	file << "Plano;Intercepto de perímetro médio" << std::endl;

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
	file << "Plano;Intercepto linear médio" << std::endl;

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
	file << "Plano;Intercepto de área médio" << std::endl;

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

		file.close();

	}
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}