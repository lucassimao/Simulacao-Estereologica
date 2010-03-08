#ifndef RETA_DE_TESTE
#define RETA_DE_TESTE

#include <vector>
#include "..\..\defs.h"

using std::vector;

namespace simulacao{

	namespace model{

		namespace grade{

			class RetaDeTeste{
			private:
				vector<Ponto> pontos;
			public:
				Ponto linhaInicio;
				Ponto linhaFim;

				RetaDeTeste(Ponto linhaInicio, Ponto linhaFim, int qtdeDePontos);
				
				int getQtdeDePontosNoIntervalo(Ponto,Ponto);
				inline vector<Ponto>::const_iterator getPontosIterator(){ return pontos.begin();}
				inline vector<Ponto>::const_iterator getPontosIteratorEnd(){ return pontos.end();}

			};
		}

	}
}

#endif