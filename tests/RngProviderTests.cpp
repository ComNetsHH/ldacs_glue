//
// Created by seba on 6/16/21.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../RngProvider.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

class RngProviderTests : public CppUnit::TestFixture {
private:
	class TesterClass : public IRng {};

	TesterClass* tester;

public:
	void setUp() override {
		tester = new TesterClass();
	}

	void tearDown() override {
		delete tester;
	}

	void testGetInts() {
		int min = 0, max = 11;
		CPPUNIT_ASSERT_NO_THROW(tester->getRandomInt(min, max));
		size_t num_tries = 10000;
		double sum = 0;
		for (size_t i = 0; i < num_tries; i++) {
			int random_int = tester->getRandomInt(min, max);
			CPPUNIT_ASSERT(0 <= random_int);
			CPPUNIT_ASSERT(random_int < max);
			sum += random_int;
		}
		sum /= num_tries;
		double mean = ((max-1) - min) / 2.0; // max is exclusive, hence -1
		double tolerance = .1;
		CPPUNIT_ASSERT(mean - tolerance <= sum);
		CPPUNIT_ASSERT(mean + tolerance >= sum);
	}

	CPPUNIT_TEST_SUITE(RngProviderTests);
		CPPUNIT_TEST(testGetInts);
	CPPUNIT_TEST_SUITE_END();
};