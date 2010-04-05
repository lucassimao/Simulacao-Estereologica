#include <gtest/gtest.h>
#include <utils/Vetor.h>
#include <defs.h>

using namespace simulacao::utils;

namespace {

	TEST(VetorTest, testarConstrutorPadrao) {
		Vetor v;
		
		ASSERT_EQ(v.x,0.0);
		ASSERT_EQ(v.y,0.0);
		ASSERT_EQ(v.z,0.0);
	}

	TEST(VetorTest, testarConstrutorComParametros) {
		Vetor v(1,3,-4);
		
		ASSERT_EQ(v.x,1.0);
		ASSERT_EQ(v.y,3.0);
		ASSERT_EQ(v.z,-4.0);

		Ponto p1 = {2.0,3.0,-4.54};
		Vetor v2(p1);
		
		ASSERT_EQ(v2.x,2.0);
		ASSERT_EQ(v2.y,3.0);
		ASSERT_EQ(v2.z,-4.54);

	}

	TEST(VetorTest, testarOperadoresDeComparacao) {
		Vetor v1(1,3,-4);
		Vetor v2(1,3,-4);
		Vetor v3(-1,-3,4);

		bool res1 = (v1==v2);
		bool res2 = (v1==v3);

		ASSERT_TRUE(res1);
		ASSERT_FALSE(res2);


	}

	TEST(VetorTest, testarSoma) {
		Vetor v1(1,3,-4);
		Vetor v2(8,7.3,-2);
		Vetor v3 = v1+v2;

		ASSERT_EQ(v3.x, v1.x+v2.x);
		ASSERT_EQ(v3.y, v1.y+v2.y);
		ASSERT_EQ(v3.z, v1.z+v2.z);


	}

	TEST(VetorTest, testarSubtracao) {
		Vetor v1(1,3,-4);
		Vetor v2(8,7.3,-2);
		Vetor v3 = v1-v2;

		ASSERT_EQ(v3.x, v1.x-v2.x);
		ASSERT_EQ(v3.y, v1.y-v2.y);
		ASSERT_EQ(v3.z, v1.z-v2.z);


	}

	TEST(VetorTest, testarMultiplicacao) {
		Vetor v1(1,3,-4);
		Vetor v2 = v1 * 2.50;

		ASSERT_EQ(v2.x, v1.x * 2.5);
		ASSERT_EQ(v2.y, v1.y * 2.5);
		ASSERT_EQ(v2.z, v1.z * 2.5);


	}


}
