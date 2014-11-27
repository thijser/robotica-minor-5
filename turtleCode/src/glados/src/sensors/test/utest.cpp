// Include code-to-test in a terrible unportable way.
#include "../switchSensor.h"
#include "../RFIDSensor.h"
#include "../ultrasoneSensor.h"
// Bring in gtest
#include <gtest/gtest.h>
#include <string>
// Declare tests

//////////
//SWITCHSENSOR
//////////

TEST(SwitchSensor, defaultConstructor)
{
	SwitchSensor s;
	std::string actual = (std::string)s.getTag();
	std::string expected = "undefined";
	ASSERT_EQ(expected, actual);
}

TEST(SwitchSensor, defaultConstructorFail)
{
	SwitchSensor s;
	std::string actual = (std::string)s.getTag();
	std::string expected = "defined";
	ASSERT_NE(expected, actual);
}

TEST(SwitchSensor, definedTag)
{
	SwitchSensor s = SwitchSensor("tagTest");
	std::string actual = s.getTag();
	std::string expected = "tagTest";
	ASSERT_EQ(expected, actual);
}

TEST(SwitchSensor, definedTagFail)
{
	SwitchSensor s = SwitchSensor("tagTest");
	std::string actual = s.getTag();
	std::string expected = "NOTtagTest";
	ASSERT_NE(expected, actual);
}

TEST(SwitchSensor, defaultState)
{
	SwitchSensor s; 
	int actual = s.getState();
	int expected = 0; 
	ASSERT_EQ(expected, actual);
}

TEST(SwitchSensor, setState)
{
	SwitchSensor s;
	s.setState(2);
	int actual = s.getState();
	int expected = 2;
	ASSERT_EQ(expected, actual);
}

TEST(SwitchSensor, defaultStateFail)
{
	SwitchSensor s;
	int actual = s.getState();
	int expected = 1;
	ASSERT_NE(expected, actual);
}

TEST(SwitchSensor, setStateFail)
{
	SwitchSensor s;
	s.setState(2);
	int actual = s.getState();
	int expected = 0;
	ASSERT_NE(expected, actual);
}

TEST(SwitchSensor, read) //PLACEHOLDER FUNCTION!
{
	SwitchSensor s;
	int actual = s.read();
	int expected = 0;
	ASSERT_EQ(expected, actual);
}

//////////
//RFIDSENSOR
//////////


TEST(RFIDSensor, defaultConstructor)
{
	RFIDSensor r;
	std::string actual = r.getTag();
	std::string expected = "rfid";
	ASSERT_EQ(expected, actual);
}

TEST(RFIDSensor, defaultConstructorFail)
{
	RFIDSensor s;
	std::string actual = (std::string)s.getTag();
	std::string expected = "defined";
	ASSERT_NE(expected, actual);
}

TEST(RFIDSensor, definedTag)
{
	RFIDSensor s = RFIDSensor("tagTest");
	std::string actual = s.getTag();
	std::string expected = "tagTest";
	ASSERT_EQ(expected, actual);
}

TEST(RFIDSensor, definedTagFail)
{
	RFIDSensor s = RFIDSensor("tagTest");
	std::string actual = s.getTag();
	std::string expected = "NOTtagTest";
	ASSERT_NE(expected, actual);
}

////////////
//UltrasoneSesor
////////////

TEST(UltrasoneSensor, defaultConstructor)
{
	UltrasoneSensor r;
	std::string actual = r.getTag();
	std::string expected = "ultrasone";
	ASSERT_EQ(expected, actual);
}

TEST(UltrasoneSensor, defaultConstructorFail)
{
	UltrasoneSensor s;
	std::string actual = (std::string)s.getTag();
	std::string expected = "defined";
	ASSERT_NE(expected, actual);
}

TEST(UltrasoneSensor, definedTag)
{
	UltrasoneSensor s = UltrasoneSensor("tagTest");
	std::string actual = s.getTag();
	std::string expected = "tagTest";
	ASSERT_EQ(expected, actual);
}

TEST(UltrasoneSensor, definedTagFail)
{
	UltrasoneSensor s = UltrasoneSensor("tagTest");
	std::string actual = s.getTag();
	std::string expected = "NOTtagTest";
	ASSERT_NE(expected, actual);
}

// Run all the tests that were declared with TEST()
int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}