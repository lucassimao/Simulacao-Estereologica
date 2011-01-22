#ifndef EXPORTADOR_DE_CORTES_SISTEMATICOS
#define EXPORTADOR_DE_CORTES_SISTEMATICOS

#include <QtDebug>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include "..\sqlite3\sqlite3.h"
#include "..\model\SimulacaoCaixa.h"
#include "..\utils\GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h"
#include "..\utils\GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy.h"
#include "..\math\ColetorDeInterceptosLinearesVisitor.h"
#include "..\math\ColetorDeAreasVisitor.h"
#include "..\math\ColetorDePontosVisitor.h"
#include "..\utils\DAO.h"
#include "..\utils\DataBaseFactory.h"
#include "..\defs.h"
#include "..\model\Parametros.h"
#include "..\utils\jpeglib\jpeglib.h"
#include "..\utils\jpeglib\jerror.h"
#define cimg_plugin "..\utils\jpeg_buffer.h"
#include "..\utils\CImg.h"
#include "..\utils\CImg.h"

using namespace simulacao::math;
using namespace simulacao::model;
using namespace simulacao::math::mathVisitor;
using namespace std;
using namespace cimg_library;
using simulacao::model::SimulacaoCaixa;

namespace simulacao{

	namespace math{

		class ExportadorDeCortesSistematicos{
			private:
				string diretorio;
				sqlite3 *db;				
				int qtdePlanos;
				bool exportarImagens;
				SimulacaoCaixa *simulacao;
				
				void exportarParaArquivo();
				void exportarPlano(int plano_pk);
				void salvarAreaDosPoligonos(int plano_pk, ofstream &outFile);
				void salvarAreaDosDiscos(int plano_pk, ofstream &outFile);
				void salvarInterceptosLineares(int plano_pk, ofstream &outFile);
				void SalvarQtdeDePontosInternos(int plano_pk, ofstream &outFile);

				
				void exportarParaImagem();
				void exportarImagemDePlanoDeCorte(int plano_pk,double largura);
				void exportarImagemDosDiscos(int plano_pk, CImg<double> &img,double zoom,double translacaoOrigem);
				void exportarImagemDosPoligonos(int plano_pk, CImg<double> &img,double zoom,double translacaoOrigem);
				void renderizarPoligono(int poligonoPk, CImg<double> &img,double zoom,double translacaoOrigem,Cor cor);

			public:
				ExportadorDeCortesSistematicos(string &diretorio, int qtdePlanos,SimulacaoCaixa *simulacao,bool exportarImagens);
				void exportar();
				
		
		};

	}

}

#endif	EXPORTADOR_DE_CORTES_SISTEMATICOS