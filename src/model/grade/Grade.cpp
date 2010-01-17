#include "Grade.h"

using namespace simulacao::model::grade;

Grade::Grade(float z0, float z1, float altura,int linhas,int pontosPorLinha){
	float deltaZ =  (abs(z0 - z1) +1)/linhas;

	for(int i=0;i<linhas;++i){
		double z = z0 - deltaZ*i;
		Ponto p0 = {-10,altura, z};
		Ponto p1 = {10,altura, z};

		RetaDeTeste l(p0,p1,pontosPorLinha);
		this->linhas.push_back(l);
	}
	

}

vector<RetaDeTeste> Grade::getLinhasNoIntervalo(double z0, double z1){
	vector<RetaDeTeste> linhasNoIntervalo;
	vector<RetaDeTeste>::const_iterator v = getLinhasIterator();
	
	while(v != getLinhasIteratorEnd()){
		RetaDeTeste l = *v;
		if (l.linhaInicio.z <= z0 &&  l.linhaInicio.z >= z1)
			linhasNoIntervalo.push_back(l);
		++v;
	}
	return linhasNoIntervalo;
}
