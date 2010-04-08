#include "AdicionarObjetosCommand.h"
#include "Parametros.h"
#include <cmath>
using namespace simulacao::model;

AdicionarObjetosCommand::AdicionarObjetosCommand(SimulacaoCaixa *simulacao, double porcentagemFaseSolida){
	this->simulacao = simulacao;
	this->porcentagemFaseSolida = porcentagemFaseSolida;
}

void AdicionarObjetosCommand::adicionarEsferas(double raio, double porcentagem, Cor cor){
	EsferaDesc desc;
	desc.cor = cor;
	desc.raio = raio;
	double volumeTotalDaCaixa = pow(this->simulacao->getArestaDaCaixa(),3);
	double volumeDaFaseSolida = (volumeTotalDaCaixa * porcentagemFaseSolida)/100.0;

	double volumeTotalDasEsferas = (porcentagem*volumeDaFaseSolida)/100.0;
	double volumeDeUmaUnicaEsfera = 4*(3.14*pow(raio,3))/3.0;
	desc.qtde = volumeTotalDasEsferas/volumeDeUmaUnicaEsfera;

	this->esferas.push_back(desc);

}

void AdicionarObjetosCommand::adicionarPrismas(double L0, double porcentagem, Cor cor, double razaoDeAspecto, double razaoDeTruncamento){
	PrismaDesc desc;
	desc.cor = cor;
	desc.L0 = L0;
	desc.razaoDeAspecto = razaoDeAspecto;
	desc.razaoDeTruncamento = razaoDeTruncamento;

	double volumeTotalDaCaixa = pow(this->simulacao->getArestaDaCaixa(),3);
	double volumeDaFaseSolida = (volumeTotalDaCaixa * porcentagemFaseSolida)/100.0;

	double volumeTotalDosPrismas = (porcentagem*volumeDaFaseSolida)/100.0;
	// para calcular o volume de um prisma triangular truncado
	// eu primeiro calculo o volume de um prisma triangular normal (area da base vezes a altura)
	// logo em seguida subtraio o volume equivalente dos cantos que são retirados
	// para criar o truncamento
	double alturaDoPrisma = L0 * razaoDeAspecto;
	double volumeDeUmPrismaTriangular = alturaDoPrisma * (pow(L0,2)*sqrt(3.0))/4.0;

	// agora calculando o volume do prisma que será retirado de cada vértice do prisma
	// triangular p/ criar o prisma triangular truncado
	double alturaDoPrismaDoVertice = L0 * razaoDeAspecto;
	double arestaDoPrismaDoVertice = razaoDeTruncamento*L0; // parâmetro L1;
	double volumeDeUmPrismaTriangularDoVertice = alturaDoPrismaDoVertice * (pow(arestaDoPrismaDoVertice,2)*sqrt(3.0))/4.0;

	double volumeDeUmPrismaTriangularTruncado = volumeDeUmPrismaTriangular - 3*volumeDeUmPrismaTriangularDoVertice;

	desc.qtde = volumeTotalDosPrismas/volumeDeUmPrismaTriangularTruncado;

	this->prismas.push_back(desc);

}

void AdicionarObjetosCommand::execute(){
	vector<EsferaDesc>::const_iterator iter = this->esferas.begin();
	while(iter != this->esferas.end()){
		EsferaDesc desc = *iter;

		Parametros::getInstance()->setRaioEsfera(desc.raio);
		Parametros::getInstance()->setCorEsfera(desc.cor);
		simulacao->adicionarObjeto(ESFERA,desc.qtde);
		++iter;
	}

	vector<PrismaDesc>::const_iterator iter2 = this->prismas.begin();

	while(iter2 != this->prismas.end()){
		PrismaDesc desc2 = *iter2;

		Parametros::getInstance()->setL0DoPrismaTriangularTruncado(desc2.L0);
		Parametros::getInstance()->setRazaoAspectoDoPrismaTriangularTruncado(desc2.razaoDeAspecto);
		Parametros::getInstance()->setRazaoDeTruncamentoDoPrismaTriangularTruncado(desc2.razaoDeTruncamento);
		Parametros::getInstance()->setCorDoPrismaTriangularTruncado(desc2.cor);

		//Parametros::getInstance()->getp
		simulacao->adicionarObjeto(PRISMA_TRIANGULAR_TRUNCADO,desc2.qtde);
		++iter2;
	}
	
}


