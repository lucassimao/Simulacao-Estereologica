#ifndef EXPORTADOR_PARA_IMAGEM
#define EXPORTADOR_PARA_IMAGEM

#include <QtDebug>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cassert>
#include "..\sqlite3\sqlite3.h"
#include "..\defs.h"
#include "..\utils\jpeglib\jpeglib.h"
#include "..\utils\jpeglib\jerror.h"
#define cimg_plugin "..\utils\jpeg_buffer.h"
#include "..\utils\CImg.h"
#include "..\utils\CImg.h"

using namespace std;
using namespace cimg_library;

#define PI 3.1415926

namespace simulacao{
	namespace exportador{

		class ExportadorParaImagem{
		
		public:
			ExportadorParaImagem(string &pastaDestino, sqlite3* db);
			void exportar();
		private:
			string destino;
			sqlite3 *db;

			void exportarImagemDePlanoDeCorte(int plano_pk,double largura,Cor corDoPlano);
			void renderizarGrade(int plano_pk, CImg<double> &img,double zoom,double translacaoOrigem);
			void exportarImagemDosDiscos(int plano_pk, CImg<double> &img,double zoom,double translacaoOrigem);
			void exportarImagemDosPoligonos(int plano_pk, CImg<double> &img,double zoom,double translacaoOrigem);
			void renderizarPoligono(int poligonoPk, CImg<double> &img,double zoom,double translacaoOrigem,Cor cor);

		};
	}
}


#endif