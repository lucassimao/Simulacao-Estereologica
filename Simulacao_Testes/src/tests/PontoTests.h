#include <gtest/gtest.h>
#include <defs.h>

using namespace simulacao::utils;

namespace {

	TEST(PontoTest, testarOperadorDeIgualdade) {
		Ponto p1;
		p1.x = 10;
		p1.y = 20;
		p1.z = -30;

		Ponto p2;
		p2.x = 10;
		p2.y = 20;
		p2.z = -30;

		ASSERT_TRUE(p1 == p2);
	}

	TEST(PontoTest, testarOperadorDeDesigualdade) {
		Ponto p1;
		p1.x = 10;
		p1.y = -20;
		p1.z = -30;

		Ponto p2;
		p2.x = 10;
		p2.y = 20;
		p2.z = -30;

		ASSERT_TRUE(p1 != p2);
	}

}
