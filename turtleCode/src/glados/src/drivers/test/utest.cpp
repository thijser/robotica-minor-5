// Include code-to-test in a terrible unportable way.
#include <launchdriver.cpp>
#include <RFIDSensor.cpp>
#include <ultrasoneSensor.cpp>
// Bring in gtest
#include <gtest/gtest.h>
#include <string>
// Declare tests

//////////
//LAUNCHDRIVER
//////////

TEST(LaunchDriver, defaultConstructor)
{
	LaunchDriver s;
	std::string actual = (std::string)s.getTag();
	std::string expected = "undefined";
	ASSERT_EQ(expected, actual);
}

// Run all the tests that were declared with TEST()
int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}