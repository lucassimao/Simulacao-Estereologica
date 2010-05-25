#include <vector>
#include <QtDebug>
#include <assert.h>
#include "ColetorDePontosVisitor.h"

#include "..\model\interceptos\Disco.h"
#include "..\model\interceptos\Poligono.h"
#include "..\defs.h"


using std::vector;
using namespace simulacao::math::mathVisitor;
using namespace simulacao::model::interceptos;

ColetorDePontosVisitor::ColetorDePontosVisitor(Grade *grade){
	this->grade = grade;
	this->qtdePontos = 0;
}

void ColetorDePontosVisitor::visit(Disco *disco){
	double z0 = disco->centro.z + disco->raio;
	double z1 = disco->centro.z - disco->raio;

	vector<RetaDeTeste> linhas = this->grade->getLinhasNoIntervalo(z0,z1);
	vector<RetaDeTeste>::const_iterator iterator = linhas.begin();


        while(iterator!=linhas.end()){
            RetaDeTeste l = *iterator;
            
            double distanciaEntreALinhaEOCentroDoDisco = abs(disco->centro.z - l.linhaInicio.z);
            double comprimentoDoInterceptoLinear = 2*sqrt(pow(disco->raio,2)-pow(distanciaEntreALinhaEOCentroDoDisco,2));
            
            Ponto interceptoInicio = {disco->centro.x - comprimentoDoInterceptoLinear/2.0,l.linhaInicio.y,l.linhaInicio.z};
            Ponto interceptoFim = {disco->centro.x + comprimentoDoInterceptoLinear/2.0,l.linhaInicio.y,l.linhaInicio.z};
			
			this->qtdePontos += l.getQtdeDePontosNoIntervalo(interceptoInicio,interceptoFim);

            ++iterator;
        }

}

void ColetorDePontosVisitor::visit(Poligono *poligono){
	double z0 = poligono->verticeComMaiorZ.z;
	double z1 = poligono->verticeComMenorZ.z;
	
	vector<RetaDeTeste> linhas = this->grade->getLinhasNoIntervalo(z0,z1);
	vector<RetaDeTeste>::const_iterator iterator = linhas.begin();

    while(iterator!=linhas.end()){
		
		RetaDeTeste retaDeTeste = *iterator;

        // já me retorna as arestas que estão interceptadas pela reta de teste
        list<SegmentoDeReta> arestasInterceptadas = poligono->getArestasInterceptadas(retaDeTeste);

		// assegura que a reta de teste intercepta duas arestas do poligono em analise
		if (arestasInterceptadas.size()==2){
			list<SegmentoDeReta>::const_iterator iter = arestasInterceptadas.begin();
			
			Ponto p0, p1;
			
			SegmentoDeReta seg1 = *iter;
			
			seg1.interceptar(retaDeTeste,&p0);
			
			++iter;
			SegmentoDeReta seg2 = *iter;
			seg2.interceptar(retaDeTeste,&p1);
			
			if (p0.x <= p1.x) // o calculo anterior ñ garante a ordem dos pontos
				this->qtdePontos +=  retaDeTeste.getQtdeDePontosNoIntervalo(p0,p1);
			else
				this->qtdePontos +=  retaDeTeste.getQtdeDePontosNoIntervalo(p1,p0);
		
		}
        ++iterator;
    }

}