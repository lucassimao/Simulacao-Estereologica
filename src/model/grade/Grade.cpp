#include "Grade.h"
#include "..\Parametros.h"
#include <cmath>

using namespace simulacao::model;
using namespace simulacao::model::grade;

Grade::Grade(float z0, float z1, float altura,int linhas,int pontosPorLinha){
	double deltaZ =  (abs(z0) + abs(z1))/linhas;
	double larguraDaCaixa = Parametros::getInstance()->getArestaDaCaixa()/2.0;

	for(int i=0;i<linhas;++i){
		double z = z0 - deltaZ*i ;
		Ponto p0 = {-larguraDaCaixa,altura, z};
		Ponto p1 = {larguraDaCaixa,altura, z};

		RetaDeTeste l(p0,p1,pontosPorLinha);
		this->linhas.push_back(l);
	}
	

}

vector<RetaDeTeste> Grade::getLinhasNoIntervalo(double z0, double z1){
	vector<RetaDeTeste> linhasNoIntervalo;
	vector<RetaDeTeste>::const_iterator v = getLinhasIterator();
	
	while(v != getLinhasIteratorEnd()){
		RetaDeTeste l = *v;
		double coordenadaZDaRetaDeTeste = l.linhaInicio.z;
		if (coordenadaZDaRetaDeTeste <= z0 &&  coordenadaZDaRetaDeTeste >= z1)
			linhasNoIntervalo.push_back(l);
		++v;
	}
	
	return linhasNoIntervalo;
}

void Grade::calcularQtdeDeLinhasEPontos(double distanciaEntrePontos,double distanciaEntreLinhas,
										int *linhasOut,int *pontosOut){

	double z0 = Parametros::getInstance()->getArestaDaCaixa()/2.0 - 1;
	double z1 =  - Parametros::getInstance()->getArestaDaCaixa()/2.0 - 1;

	*linhasOut = floor(((z0) + abs(z1) +1)/distanciaEntreLinhas);
	*pontosOut = floor(Parametros::getInstance()->getArestaDaCaixa()/distanciaEntrePontos);


}
void Grade::calcularDistanciaEntreLinhasEDistanciaEntrePontos(int qtdeLinhas,int qtdePontos,
															  double *distanciaEntrePontos,double *distanciaEntreLinhas){

	double z0 = Parametros::getInstance()->getArestaDaCaixa()/2.0 - 1;
	double z1 =  - Parametros::getInstance()->getArestaDaCaixa()/2.0 - 1;
	
	*distanciaEntrePontos = Parametros::getInstance()->getArestaDaCaixa()/(double)qtdePontos;
	*distanciaEntreLinhas = (z0 + abs(z1))/qtdeLinhas;

}
