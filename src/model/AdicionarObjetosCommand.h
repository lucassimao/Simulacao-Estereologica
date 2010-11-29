#ifndef ADICIONAR_OBJETOS_COMMAND_H
#define ADICIONAR_OBJETOS_COMMAND_H

#include <vector>
using std::vector;

#include "..\defs.h"
#include "SimulacaoCaixa.h"

namespace simulacao{
	namespace model{

		typedef struct{
			int qtde;
			Cor cor;
			double raio;
		} EsferaDesc;

		typedef struct{
			double L0;
			double razaoDeAspecto;
			double razaoDeTruncamento;
			Cor cor;
			double qtde;
		} PrismaDesc;

		
		class AdicionarObjetosCommand{
		
			public:
				AdicionarObjetosCommand(SimulacaoCaixa *simulacao, double porcentagemFaseSolida);
				void adicionarEsferas(double raio, double porcentagem, Cor cor);
				void adicionarPrismas(double L0, double porcentagem, Cor cor, double razaoDeAspecto, double razaoDeTruncamento);
				void adicionarPrismas(double L0, int quantidade, Cor cor, double razaoDeAspecto, double razaoDeTruncamento);
				void execute();
			private:
				vector<EsferaDesc> esferas;
				vector<PrismaDesc> prismas;
				SimulacaoCaixa *simulacao;
				double porcentagemFaseSolida;
		};
	}
}

#endif