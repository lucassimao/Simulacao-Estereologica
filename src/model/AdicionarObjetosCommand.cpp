#include "AdicionarObjetosCommand.h"
#include "Parametros.h"
#include "atores\PrismaTriangularTruncado.h"
#include <cmath>
using namespace simulacao::model;
using namespace simulacao::model::atores;

AdicionarObjetosCommand::AdicionarObjetosCommand(SimulacaoCaixa *simulacao, double porcentagemFaseSolida){
	this->simulacao = simulacao;
	this->porcentagemFaseSolida = porcentagemFaseSolida;
}

void AdicionarObjetosCommand::adicionarEsferas(double raio, double porcentagem, Cor cor){
	EsferaDesc desc;
	desc.cor = cor;
	desc.raio = raio;
	double volumeTotalDaCaixa = this->simulacao->getVolumeDaCaixa();
	double volumeDaFaseSolida = (volumeTotalDaCaixa * porcentagemFaseSolida)/100.0;

	double volumeTotalDasEsferas = (porcentagem*volumeDaFaseSolida)/100.0;
	double volumeDeUmaUnicaEsfera = 4*(3.14*pow(raio,3))/3.0;
	desc.qtde = volumeTotalDasEsferas/volumeDeUmaUnicaEsfera;

	this->esferas.push_back(desc);
}

void AdicionarObjetosCommand::adicionarEsferas(double raio, int quantidade, Cor cor){
	EsferaDesc desc;
	desc.cor = cor;
	desc.raio = raio;
	desc.qtde = quantidade;

	this->esferas.push_back(desc);
}

void AdicionarObjetosCommand::adicionarPrismas(double L0, double porcentagem, Cor cor, double razaoDeAspecto, double razaoDeTruncamento){
	PrismaDesc desc;
	desc.cor = cor;
	desc.L0 = L0;
	desc.razaoDeAspecto = razaoDeAspecto;
	desc.razaoDeTruncamento = razaoDeTruncamento;

	double volumeTotalDaCaixa = this->simulacao->getVolumeDaCaixa();
	double volumeDaFaseSolida = (volumeTotalDaCaixa * porcentagemFaseSolida)/100.0;
	double volumeTotalDosPrismas = (porcentagem*volumeDaFaseSolida)/100.0;
	double volumeDeUmPrismaTriangularTruncado = PrismaTriangularTruncado::calcularVolume(razaoDeAspecto,razaoDeTruncamento,L0);

	desc.qtde = volumeTotalDosPrismas/volumeDeUmPrismaTriangularTruncado;

	this->prismas.push_back(desc);

}

void AdicionarObjetosCommand::adicionarPrismas(double L0, int quantidade, Cor cor, double razaoDeAspecto, double razaoDeTruncamento){
	PrismaDesc desc;
	desc.cor = cor;
	desc.L0 = L0;
	desc.qtde = quantidade;
	desc.razaoDeAspecto = razaoDeAspecto;
	desc.razaoDeTruncamento = razaoDeTruncamento;

	this->prismas.push_back(desc);
}

void AdicionarObjetosCommand::execute(){
	vector<EsferaDesc>::const_iterator iter = this->esferas.begin();
	while(iter != this->esferas.end()){
		EsferaDesc desc = *iter;
		Parametros::getInstance()->setRaioEsfera(desc.raio);

		simulacao->adicionarEsferas(desc.qtde,desc.cor);
		++iter;
	}

	vector<PrismaDesc>::const_iterator iter2 = this->prismas.begin();

	while(iter2 != this->prismas.end()){
		PrismaDesc desc2 = *iter2;
		double l0 = desc2.L0;
		double razaoDeAspecto =  desc2.razaoDeAspecto;
		double razaoDeTruncamento = desc2.razaoDeTruncamento;
		Cor cor = desc2.cor;
		int qtde = desc2.qtde;

		if (razaoDeTruncamento == 0){
			Parametros::getInstance()->setBasePrisma(l0);
			Parametros::getInstance()->setAlturaPrisma(razaoDeAspecto*l0);		
				
			simulacao->adicionarPrismas(l0,qtde,cor,razaoDeAspecto,0);
		}else{
			Parametros::getInstance()->setL0DoPrismaTriangularTruncado(l0);
			Parametros::getInstance()->setRazaoAspectoDoPrismaTriangularTruncado(razaoDeAspecto);
			Parametros::getInstance()->setRazaoDeTruncamentoDoPrismaTriangularTruncado(razaoDeTruncamento);

			simulacao->adicionarPrismas(l0,qtde,cor,razaoDeAspecto,razaoDeTruncamento);
		}
		++iter2;
	}
	
}


