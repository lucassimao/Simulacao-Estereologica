#include <gtest/gtest.h>
#include <utils/SegmentoDeReta.h>
#include <model/grade/RetaDeTeste.h>
#include <defs.h>

using namespace simulacao::utils;
using namespace simulacao::model::grade;

namespace {

	TEST(SegmentoDeRetaTest, testarInterceptacaoComPlano1) {
		Ponto p1a = {1,1,0};
		Ponto p1b = {1,10,0};
		Ponto pontoDeInterceptacao;

		SegmentoDeReta s1(p1a,p1b);

		bool res = s1.interceptarComPlano(5,&pontoDeInterceptacao);
		ASSERT_EQ(1,pontoDeInterceptacao.x);
		ASSERT_EQ(5,pontoDeInterceptacao.y);
		ASSERT_EQ(0,pontoDeInterceptacao.z);


		ASSERT_TRUE(res);
	}

	TEST(SegmentoDeRetaTest, testarInterceptacaoComPlano2) {
		Ponto p1a = {2,1,-5};
		Ponto p1b = {2,10,5};
		Ponto pontoDeInterceptacao;

		SegmentoDeReta s1(p1a,p1b);

		bool res = s1.interceptarComPlano(5,&pontoDeInterceptacao);
		ASSERT_TRUE(res);
	}

	TEST(SegmentoDeRetaTest, testarInterceptacaoComPlano3) {
		Ponto p1a = {1,1,0};
		Ponto p1b = {1,10,0};

		SegmentoDeReta s1(p1a,p1b);

		bool res = s1.interceptarComPlano(0,NULL);

		ASSERT_FALSE(res);
	}

	TEST(SegmentoDeRetaTest, testarInterceptacaoComPlano4) {
		Ponto p1a = {1,1,0};
		Ponto p1b = {1,10,0};

		SegmentoDeReta s1(p1a,p1b);

		bool res = s1.interceptarComPlano(0.999,NULL);

		ASSERT_FALSE(res);
	}

	TEST(SegmentoDeRetaTest, testarInterceptacaoComPlano5) {
		Ponto p1a = {1,1,0};
		Ponto p1b = {1,10,0};
		SegmentoDeReta s1(p1a,p1b);

		bool res = s1.interceptarComPlano(10.001,NULL);

		ASSERT_FALSE(res);
	}
	TEST(SegmentoDeRetaTest, testarInterceptacaoComPlano6) {
		Ponto p1a = {-1,-10,-20};
		Ponto p1b = {1,10,50};
		Ponto pontoDeInterceptacao;
		SegmentoDeReta s1(p1a,p1b);

		bool res = s1.interceptarComPlano(0,&pontoDeInterceptacao);

		ASSERT_TRUE(res);
	}

	TEST(SegmentoDeRetaTest, testarInterceptacaoComRetaDeTeste1) {
		Ponto p1a = {1,-30,0};
		Ponto p1b = {1,30,0};
		Ponto pontoDeInterceptacao;

		SegmentoDeReta s1(p1a,p1b);

		Ponto retaInicio = {-20,10,0};
		Ponto retaFim = {20,10,0};
		RetaDeTeste reta1(retaInicio,retaFim,100);

		bool res = s1.interceptar(reta1,&pontoDeInterceptacao);

		ASSERT_TRUE(res);
	}

}