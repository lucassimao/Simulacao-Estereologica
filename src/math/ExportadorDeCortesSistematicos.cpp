#include "ExportadorDeCortesSistematicos.h"

#define PI 3.1415926
#define BANCO_EM_MEMORIA


ExportadorDeCortesSistematicos::ExportadorDeCortesSistematicos(string &diretorio, int qtdePlanos,SimulacaoCaixa *simulacao,bool exportarImagens){
	 	
	this->diretorio = diretorio;
	this->qtdePlanos = qtdePlanos;
	this->simulacao=simulacao;
	this->exportarImagens = exportarImagens;
	
	string bancoDeDadosFile;

	#ifdef BANCO_EM_MEMORIA
		bancoDeDadosFile = ":memory:";
	#else
		long timestamp = time(0);
		ostringstream dbFilename;
		dbFilename << diretorio << "/" << timestamp << ".db";		
		bancoDeDadosFile = dbFilename.str();
	#endif

	this->db = DataBaseFactory::getInstance()->criarBanco(bancoDeDadosFile.c_str());
    if (!this->db)
    {
		throw runtime_error( sqlite3_errmsg(db) );
	    sqlite3_close(db);
	}
}


void ExportadorDeCortesSistematicos::exportarParaImagem(){
	sqlite3_stmt *planoDeCorte_stmt = 0;
	const char *planoDeCorte_select =  "select rowid,largura from planoDeCorte order by rowid;";

	int res = sqlite3_prepare_v2(this->db,planoDeCorte_select,-1,&planoDeCorte_stmt,NULL);
	
    if( res==SQLITE_OK && planoDeCorte_stmt ){
		
		do{
			res = sqlite3_step(planoDeCorte_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			int planoPK = sqlite3_column_int(planoDeCorte_stmt,0);
			double largura = sqlite3_column_double(planoDeCorte_stmt,1);
			exportarImagemDePlanoDeCorte(planoPK,largura);
			res = sqlite3_step(planoDeCorte_stmt);
		}
		sqlite3_finalize(planoDeCorte_stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}

void ExportadorDeCortesSistematicos::exportarImagemDePlanoDeCorte(int plano_pk,double largura){
	CImg<double> img(600,600,1,3);
	double zoom = 600/largura;
	double translacaoOrigem = largura/2.0;

	Cor c = simulacao->getPlanoDeCorte()->cor;
	const unsigned char cor[] = { c.r*255, c.g*255, c.b*255 };
	img.draw_rectangle(0,0,largura*zoom,largura*zoom,cor);
	
	exportarImagemDosDiscos(plano_pk,img,zoom,translacaoOrigem);	
	exportarImagemDosPoligonos(plano_pk,img,zoom,translacaoOrigem);
	
	unsigned int buffer_size = 100*1024*1024;
	JOCTET *buffer_output = new JOCTET[buffer_size];
	img.save_jpeg_buffer(buffer_output,buffer_size);

	ostringstream  filename_output;
	filename_output << this->diretorio << "/plano_de_corte" << plano_pk << ".jpg";
	std::FILE* file_output = std::fopen(filename_output.str().c_str() ,"wb");
	std::fwrite(buffer_output, sizeof(JOCTET), buffer_size, file_output);

	std::fclose(file_output);
	delete[] buffer_output;

}


void ExportadorDeCortesSistematicos::exportarImagemDosDiscos(int plano_pk, CImg<double> &img,double zoom, double translacaoOrigem){
	sqlite3_stmt *discos_stmt = 0;
	const char *discos_select = "select raio, xcentro,zcentro,r,g,b from discos where planoDeCorte_fk = ?1;";

	int res = sqlite3_prepare_v2(this->db,discos_select,-1,&discos_stmt,NULL);
	
    if( res==SQLITE_OK && discos_stmt ){
		res = sqlite3_bind_int(discos_stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(discos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			double raio = sqlite3_column_double(discos_stmt,0);
			double xcentro = sqlite3_column_double(discos_stmt,1) + translacaoOrigem;
			double zcentro = sqlite3_column_double(discos_stmt,2) + translacaoOrigem;
			Cor c;
			c.r = sqlite3_column_double(discos_stmt,3);
			c.g = sqlite3_column_double(discos_stmt,4);
			c.b = sqlite3_column_double(discos_stmt,5);
			double cor[] = {c.r*255,c.g*255,c.b*255};
			img.draw_circle(xcentro*zoom,zcentro*zoom,raio*zoom,cor);
			res = sqlite3_step(discos_stmt);
		}
		sqlite3_finalize(discos_stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}


void ExportadorDeCortesSistematicos::exportarImagemDosPoligonos(int plano_pk, CImg<double> &img,double zoom,double translacaoOrigem){
	sqlite3_stmt *discos_stmt = 0;
	const char *discos_select =  "select rowid,r,g,b from poligonos where planoDeCorte_fk = ?1;";

	int res = sqlite3_prepare_v2(this->db,discos_select,-1,&discos_stmt,NULL);
	
    if( res==SQLITE_OK && discos_stmt ){
		res = sqlite3_bind_int(discos_stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(discos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			int poligonoPk = sqlite3_column_int(discos_stmt,0);
			Cor cor;
			cor.r = sqlite3_column_double(discos_stmt,1);
			cor.g = sqlite3_column_double(discos_stmt,2);
			cor.b = sqlite3_column_double(discos_stmt,3);
			renderizarPoligono(poligonoPk,img,zoom,translacaoOrigem,cor);
			res = sqlite3_step(discos_stmt);
		}
		sqlite3_finalize(discos_stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}

void ExportadorDeCortesSistematicos::renderizarPoligono(int poligonoPk, CImg<double> &img,double zoom,double translacaoOrigem,Cor cor){
	sqlite3_stmt *discos_stmt = 0;
	const char *discos_select = "select x,z from vertices_poligono where poligono_fk = ?1 order by posicao;";

	int res = sqlite3_prepare_v2(this->db,discos_select,-1,&discos_stmt,NULL);
	
    if( res==SQLITE_OK && discos_stmt ){
		res = sqlite3_bind_int(discos_stmt,1,poligonoPk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(discos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		CImgList<double> vertices;
		unsigned char color[] = { cor.r*255,cor.g*255,cor.b*255 };
		while (res != SQLITE_DONE){
			double x = sqlite3_column_double(discos_stmt,0) + translacaoOrigem;
			double z = sqlite3_column_double(discos_stmt,1) + translacaoOrigem;
			vertices.insert(CImg<double>::vector(x*zoom,z*zoom));
			
			res = sqlite3_step(discos_stmt);
		}
		img.draw_polygon(vertices>'x',color);


		sqlite3_finalize(discos_stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}

void ExportadorDeCortesSistematicos::salvarAreaDosPoligonos( int plano_pk, ofstream &outFile){
	sqlite3_stmt *poligonos_stmt = 0;
	ostringstream  poligonos_select;
	poligonos_select << "select rowid,area,perimetro,L0,razaoDeAspectoOriginaria,razaoDeTruncamentoOriginaria ";
	poligonos_select << "from poligonos where planoDeCorte_fk = ?1;";


	int res = sqlite3_prepare_v2(this->db,poligonos_select.str().c_str(),-1,&poligonos_stmt,NULL);
	
    if( res==SQLITE_OK && poligonos_stmt ){
		res = sqlite3_bind_int(poligonos_stmt,1,plano_pk);
		assert(res == SQLITE_OK);
		
		do{
			res = sqlite3_step(poligonos_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while(res != SQLITE_DONE){
			//int poligono_pk = sqlite3_column_int(poligonos_stmt,0);
			double area = sqlite3_column_double(poligonos_stmt,1);
			//double perimetro = sqlite3_column_double(poligonos_stmt,2);
			outFile << area << endl;
			res = sqlite3_step(poligonos_stmt);
		}	

		sqlite3_finalize(poligonos_stmt);
		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

}
void ExportadorDeCortesSistematicos::salvarAreaDosDiscos(int plano_pk, ofstream &outFile){
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

void ExportadorDeCortesSistematicos::salvarInterceptosLineares(int plano_pk, ofstream &outFile){
	sqlite3_stmt *interceptos_stmt = 0;
	ostringstream  interceptos_select;
	interceptos_select << "select tamanho from interceptosLineares where planoDeCorte_fk = ?1 order by tamanho;";


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

void ExportadorDeCortesSistematicos::exportarPlano(int planoDeCorteID){
	
	locale ptBR(locale(),new WithComma);

	ostringstream arquivoAreasDoPlano;
	arquivoAreasDoPlano << this->diretorio << "/areas_plano_" << planoDeCorteID << ".csv"; 
	ofstream areasFile(arquivoAreasDoPlano.str().c_str(),std::ios::out);
	areasFile.imbue(ptBR);

	// agora salvando os interceptos de área
	salvarAreaDosPoligonos(planoDeCorteID,areasFile);
	salvarAreaDosDiscos(planoDeCorteID,areasFile);
	areasFile.close();


	// agora salvando os interceptos lineares
	ostringstream arquivoInterceptosLineares;
	arquivoInterceptosLineares << this->diretorio << "/interceptosLineares_plano_" << planoDeCorteID << ".csv"; 
	ofstream interceptosLinearesFile(arquivoInterceptosLineares.str().c_str(),std::ios::out);
	interceptosLinearesFile.imbue(ptBR);

	salvarInterceptosLineares(planoDeCorteID,interceptosLinearesFile);
	interceptosLinearesFile.close();

	// exportando a quantidade de pontos
	ostringstream arquivoQtdePontos;
	arquivoQtdePontos << this->diretorio << "/pontosInternos_plano_" << planoDeCorteID << ".csv"; 
	ofstream pontosInternosFile(arquivoQtdePontos.str().c_str(),std::ios::out);
	pontosInternosFile.imbue(ptBR);

	SalvarQtdeDePontosInternos(planoDeCorteID,pontosInternosFile);
	pontosInternosFile.close();
}

void ExportadorDeCortesSistematicos::SalvarQtdeDePontosInternos(int plano_pk, ofstream &outFile){
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

void ExportadorDeCortesSistematicos::exportarParaArquivo(){
	sqlite3_stmt *planoDeCorte_stmt = 0;
	ostringstream  planoDeCorte_select;
	planoDeCorte_select << "select rowid,altura from planoDeCorte;";


	int res = sqlite3_prepare_v2(this->db,planoDeCorte_select.str().c_str(),-1,&planoDeCorte_stmt,NULL);
	
    if( res==SQLITE_OK && planoDeCorte_stmt ){
		
		do{
			res = sqlite3_step(planoDeCorte_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			int planoPK = sqlite3_column_int(planoDeCorte_stmt,0);
			//double altura = sqlite3_column_double(planoDeCorte_stmt,1);
			exportarPlano(planoPK);
			res = sqlite3_step(planoDeCorte_stmt);
		}
		sqlite3_finalize(planoDeCorte_stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;

}

void ExportadorDeCortesSistematicos::exportar(){
	Parametros *p = Parametros::getInstance();
	double h0 = p->getAlturaDaBaseDaCaixa();
	double h1 = p->getArestaDaCaixa() + p->getAlturaDaBaseDaCaixa();
	simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy(h0,h1,this->qtdePlanos));

	DAO dao(this->db);
	
	NxActor* caixa = simulacao->getCaixa();
	NxActor* planoDeCorte = simulacao->getPlanoDeCorte()->getNxActor();
	
	Parametros *params = Parametros::getInstance();
	int qtdeLinhaNaGrade = params->getQtdeLinhasNaGrade()* params->getQtdePontosPorLinhaNaGrade();
	double larguraDoPlanoDeCorte = params->getLarguraDoPlanoDeCorte();
	
	double volumeFaseSolida = simulacao->getVolumeFaseSolida();
	double volumeFaseLigante = simulacao->getVolumeFaseLigante();

	for(int i=0;i< this->qtdePlanos;++i){
		
		simulacao->novoPlanoDeCorte();

		NxU32 qtdeAtores = simulacao->getQtdeObjetos();
		NxActor** atores = simulacao->getAtores();
		vector<Intercepto*> interceptos;
		
		NxVec3 planoGlobalPosition = planoDeCorte->getGlobalPosition();

		ColetorDeInterceptosLinearesVisitor *visitor1 = new ColetorDeInterceptosLinearesVisitor(simulacao->getGrade());
		ColetorDePontosVisitor *visitor2 = new ColetorDePontosVisitor(simulacao->getGrade());
		ColetorDeAreasVisitor *visitor3 = new ColetorDeAreasVisitor(simulacao->getGrade());
		
		__int64 planoID = dao.salvarPlano(planoGlobalPosition.y,larguraDoPlanoDeCorte);

		while (qtdeAtores--)
		{
			NxActor* ator = *atores++;

			if (ator != caixa && ator!= planoDeCorte){
				Ator *a = (Ator *)ator->userData;	

				if (a->estaInterceptadoPeloPlano(planoGlobalPosition)){
					Intercepto *intercepto = a->getIntercepto(planoGlobalPosition);

					intercepto->accept(visitor1);
					intercepto->accept(visitor2);
					intercepto->accept(visitor3);

					switch(intercepto->getType()){
					case Type_Disco:
						dao.salvarDisco(planoID,static_cast<Disco*>(intercepto));
						break;
					case Type_Poligono:
						dao.salvarPoligono(planoID,static_cast<Poligono*>(intercepto));
						break;
					}
				}
			}
		}
		dao.salvarInterceptosLineares(planoID,visitor1);
		dao.salvarEstatisticas(planoID,visitor3->getAreaTotalColetada(),400,
			visitor2->getQtdeDePontosInternosAInterceptosDeArea(),qtdeLinhaNaGrade,volumeFaseSolida,volumeFaseLigante);
	}

	exportarParaArquivo();
	if (this->exportarImagens){
		exportarParaImagem();
	}
	simulacao->setGeradorDeAlturaDoPlanoStrategy(new GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy());
	sqlite3_close(db);

}
		
