#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

enum TipoDeGrao{
	ESFERA,CUBO,PRISMA_TRIANGULAR_TRUNCADO,PRISMA_TRIANGULAR
};

enum Status{
	PAUSADO, EXECUCAO,PARADO
};

typedef struct{
	double x,y,z;
}Ponto;

typedef struct {
	double r,g,b;
} Cor;

#define VERMELHO {1.0,0,0}


#endif
