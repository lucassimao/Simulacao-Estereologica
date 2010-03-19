#include <gtest/gtest.h>
#include <utils/GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy.h>
#include <utils/GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy.h>

using namespace simulacao::utils;

namespace {

	TEST(GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategyTest, testarIntervalo) {
		float alturaInicial = 5.5f;
		float alturaFinal = 24.9f;
		GeradorDeAlturaAleatoriaDoPlanoDeCorteStrategy strategy(alturaInicial,alturaFinal);

		for(int i=0;i<10000;++i){
			float altura = strategy.novaAltura();
			ASSERT_GE(altura,alturaInicial);
			ASSERT_LE(altura,alturaFinal);
		}

		
	}

	TEST(GeradorDeAlturaSistematicaDeCorteStrategyTest, testarSistematica) {
		float alturaInicial = 0.0f;
		float alturaFinal = 100.0f;
		int qtdePlanos = 10;
		GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy strategy(alturaInicial,alturaFinal,qtdePlanos);
		
		ASSERT_EQ(alturaInicial,strategy.novaAltura());

		for(int i=1;i<=10000;++i){
			float f = strategy.novaAltura();
			if (i%10 == 0)
				ASSERT_EQ(alturaInicial,f);
		}		
		
	}

	TEST(GeradorDeAlturaSistematicaDeCorteStrategyTest, testarQuantidadeDePlanos) {
		float alturaInicial = 0.0f;
		float alturaFinal = 100.0f;
		int qtdePlanos = 30;
		GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy strategy(alturaInicial,alturaFinal,qtdePlanos);
		
		int qtde = 0;

		for(int i=0;i<30;++i){
			strategy.novaAltura();
		}		
		ASSERT_EQ(alturaInicial,strategy.novaAltura());	
	}

}  
