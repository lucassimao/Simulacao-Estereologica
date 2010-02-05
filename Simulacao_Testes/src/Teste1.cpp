#include <gtest/gtest.h>


namespace {

	TEST(FooTest, Besteira) {
		ASSERT_EQ(1,1);
	}

	TEST(FooTest, TesteBesteira) {
		ASSERT_EQ(10*(2/5),0);
	}

}  

int main(int argc,char **argv){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}