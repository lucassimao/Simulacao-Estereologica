#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

enum TipoDeGrao{
	ESFERA,CUBO,PRISMA_TRIANGULAR_TRUNCADO,PRISMA_TRIANGULAR
};

enum Status{
	PAUSADO, EXECUCAO
};

typedef struct{
	double x,y,z;
}Ponto;

#endif
