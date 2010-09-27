#define TEST_ENV

#include <gtest/gtest.h>
#include "tests/GeradorDeAlturaDoPlanoDeCorteStrategyTests.h"
#include "tests/VetorTests.h"
#include "tests/SegmentoDeRetaTests.h"
#include "tests/PoligonoTests.h"
#include "tests/JarvisMarchTests.h"
#include "tests/PontoTests.h"

int main(int argc,char **argv){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}