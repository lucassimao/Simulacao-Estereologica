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

	TEST(GeradorDeAlturaSistematicaDeCorteStrategyTest, testarDelta) {
		float alturaInicial = 0.0f;
		float alturaFinal = 100.0f;
		int qtdePlanos = 10;
		GeradorSistematicoDeAlturaDoPlanoDeCorteStrategy strategy(alturaInicial,alturaFinal,qtdePlanos);
		
		float h0 = strategy.novaAltura();
		ASSERT_EQ(h0,0);

		float h1 = strategy.novaAltura();

		float delta = h1 - h0;
		ASSERT_EQ(delta,10);

		for(int i=2;i<=10000;++i){
			if (i%10 == 0)
				ASSERT_EQ(alturaInicial,strategy.novaAltura());
			else
				strategy.novaAltura();
		}		
		
	}

}  
