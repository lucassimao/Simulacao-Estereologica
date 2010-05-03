#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <locale>

using namespace std;

enum TipoDeGrao{
	ESFERA,PRISMA_TRIANGULAR_TRUNCADO,PRISMA_TRIANGULAR
};

enum Status{
	PAUSADO, EXECUCAO,PARADO
};

enum RenderizacaoStrategyType{
	RenderizarInterceptos,RenderizarAtores
};



typedef struct{
	double x,y,z;
}Ponto;


typedef struct {
	float r,g,b;
} Cor;

#define VERMELHO {1.0,0,0}

// classe que permite numeros decimais serem salvos em arquivo usando 
// virgula com seprador decimal
class WithComma: public numpunct<char> 
{
	protected: char do_decimal_point() const { return ','; } 
};

#endif
