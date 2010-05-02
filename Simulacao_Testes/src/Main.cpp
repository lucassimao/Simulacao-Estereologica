#include <gtest/gtest.h>
#include "tests/GeradorDeAlturaDoPlanoDeCorteStrategyTests.h"
#include "tests/VetorTests.h"
#include "tests/SegmentoDeRetaTests.h"
#include "tests/PoligonoTests.h"
#include "tests/GrahamTests.h"

int main(int argc,char **argv){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}