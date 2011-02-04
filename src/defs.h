#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <locale>
#include <iostream>

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

enum InterceptoType{
	Type_Disco, Type_Poligono
};


typedef struct tponto Ponto;

struct tponto{
	double x,y,z;

	bool operator==(Ponto ponto) const{
		return (ponto.x == this->x) && (ponto.y == this->y) && (ponto.z == this->z);
	}
	bool operator!=(Ponto ponto) const{
		return (ponto.x != this->x) || (ponto.y != this->y) || (ponto.z != this->z);
	}
};

typedef struct {
	float r,g,b;
} Cor;

#define VERMELHO {1.0f,0,0}
#define VERDE {0,1.0f,0}

// classe que permite numeros decimais serem salvos em arquivo usando 
// virgula com seprador decimal
class WithComma: public numpunct<char> 
{
	protected: char do_decimal_point() const { return ','; } 
};

#endif
