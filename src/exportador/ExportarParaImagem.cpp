#include "ExportadorParaImagem.h"
#include "..\model\Parametros.h"
#include "..\model\grade\Grade.h"

using namespace simulacao::exportador;
using namespace simulacao::model;
using namespace simulacao::model::grade;

ExportadorParaImagem::ExportadorParaImagem(string &pastaDestino, sqlite3* db,bool renderizarPontos,bool renderizarLinhas){
	this->destino = pastaDestino;
	this->db = db;
	this->renderizarPontos = renderizarPontos;
	this->renderizarLinhas = renderizarLinhas;
}

void ExportadorParaImagem::exportar(){
	sqlite3_stmt *planoDeCorte_stmt = 0;
	const char *planoDeCorte_select =  "select rowid,largura,r,g,b from planoDeCorte order by rowid;";

	int res = sqlite3_prepare_v2(this->db,planoDeCorte_select,-1,&planoDeCorte_stmt,NULL);
	
    if( res==SQLITE_OK && planoDeCorte_stmt ){
		
		do{
			res = sqlite3_step(planoDeCorte_stmt);
		}
		while(res != SQLITE_ROW && res != SQLITE_DONE);
		
		while (res != SQLITE_DONE){
			Cor corDoPlano;
			int planoPK = sqlite3_column_int(planoDeCorte_stmt,0);
			double largura = sqlite3_column_double(planoDeCorte_stmt,1);
			corDoPlano.r = sqlite3_column_double(planoDeCorte_stmt,2);
			corDoPlano.g = sqlite3_column_double(planoDeCorte_stmt,3);
			corDoPlano.b = sqlite3_column_double(planoDeCorte_stmt,4);

			exportarImagemDePlanoDeCorte(planoPK,largura,corDoPlano);
			res = sqlite3_step(planoDeCorte_stmt);
		}
		sqlite3_finalize(planoDeCorte_stmt);		
    }
	else
		qDebug() <<  sqlite3_errmsg(this->db)<<endl;
}

void ExportadorParaImagem::exportarImagemDePlanoDeCorte(int plano_pk,double largura,Cor corDoPlano){
	CImg<double> img(600,600,1,3);
	double zoom = 600/largura;
	double translacaoOrigem = largura/2.0;

	const unsigned char cor[] = { corDoPlano.r*255, corDoPlano.g*255, corDoPlano.b*255 };
	img.draw_rectangle(0,0,largura*zoom,largura*zoom,cor);
	
	exportarImagemDosDiscos(plano_pk,img,zoom,translacaoOrigem);	
	exportarImagemDosPoligonos(plano_pk,img,zoom,translacaoOrigem);
	
	if (this->renderizarLinhas || this->renderizarPontos){
		renderizarGrade(plano_pk,img,zoom,translacaoOrigem);
	}
	unsigned int buffer_size = 100*1024*1024;
	JOCTET *buffer_output = new JOCTET[buffer_size];
	img.save_jpeg_buffer(buffer_output,buffer_size);

	ostringstream  filename_output;
	filename_output << this->destino << "/plano_de_corte" << plano_pk << ".jpg";
	std::FILE* file_output = std::fopen(filename_output.str().c_str() ,"wb");
	std::fwrite(buffer_output, sizeof(JOCTET), buffer_size, file_output);

	std::fclose(file_output);
	delete[] buffer_output;

}


void ExportadorParaImagem::renderizarGrade(int plano_pk, CImg<double> &img,double zoom,double translacaoOrigem){
	int qtdeLinha = Parametros::getInstance()->getParametrosDaGrade().qtdeLinhas; 
	int qtdePontos = Parametros::getInstance()->getParametrosDaGrade().qtdePontos;
	float z0 = (Parametros::getInstance()->getArestaDaCaixa())/2.0f;
	float z1 = -1*z0;
	Grade grade(z0, z1,plano_pk,qtdeLinha,qtdePontos);

	vector<RetaDeTeste>::const_iterator iterator= grade.getLinhasIterator();
	unsigned char blue[] = { 0,0,255 };
	unsigned char branco[] = { 255,255,255 };
	
	while (iterator != grade.getLinhasIteratorEnd()){
		RetaDeTeste reta = *iterator;
		
		if (this->renderizarLinhas){
			double x0 = reta.linhaInicio.x + translacaoOrigem;
			double z0 = reta.linhaInicio.z + translacaoOrigem;
			
			double x1 = reta.linhaFim.x + translacaoOrigem;
			double z1 = reta.linhaFim.z + translacaoOrigem;
			
			img.draw_line(x0*zoom,z0*zoom,x1*zoom,z1*zoom,blue);
		}

		if (this->renderizarPontos){
			vector<Ponto>::const_iterator pontosIterator = reta.getPontosIterator();
			while(pontosIterator != reta.getPontosIteratorEnd()){
				Ponto p = *pontosIterator;
				double x = p.x + translacaoOrigem;
				double z = p.z + translacaoOrigem;

				img.draw_circle(x*zoom,z*zoom,2,branco);
				pontosIterator++;
			}
		}

		++iterator;
	}
}
void ExportadorParaImagem::exportarImagemDosPoligonos(int plano_pk, CImg<double> &img,double zoom,double translacaoOrigem){
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

void ExportadorParaImagem::renderizarPoligono(int poligonoPk, CImg<double> &img,double zoom,double translacaoOrigem,Cor cor){
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

void ExportadorParaImagem::exportarImagemDosDiscos(int plano_pk, CImg<double> &img,double zoom, double translacaoOrigem){
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

