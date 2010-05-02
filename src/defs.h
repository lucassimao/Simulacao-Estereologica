#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

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


#endif
