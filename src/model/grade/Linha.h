#ifndef LINHA_H
#define LINHA_H

#include <vector>
#include "..\..\defs.h"

using std::vector;

namespace simulacao{

	namespace model{

		namespace grade{

			class Linha{
			private:
				vector<Ponto> pontos;
			public:
				Ponto linhaInicio;
				Ponto linhaFim;

				Linha(Ponto linhaInicio, Ponto linhaFim, int qtdeDePontos);
				inline vector<Ponto>::const_iterator getPontosIterator(){ return pontos.begin();}
				inline vector<Ponto>::const_iterator getPontosIteratorEnd(){ return pontos.end();}

			};
		}

	}
}

#endif