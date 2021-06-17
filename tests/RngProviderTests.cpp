//
// Created by seba on 6/16/21.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../RngProvider.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

namespace TUHH_INTAIRNET_MCSOTDMA {
	class RngProviderTests : public CppUnit::TestFixture {
	private:
		class TesterClass : public IRng {
		};

		TesterClass* tester;

	public:
		void setUp() override {
			RngProvider::getInstance().reset();
		}

		void tearDown() override {
			delete tester;
			RngProvider::getInstance().reset();
		}

		void testGetInts() {
			RngProvider::getInstance().setUseDefaultRngs(true);
			tester = new TesterClass();
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
			double mean = ((max - 1) - min) / 2.0; // max is exclusive, hence -1
			double tolerance = .1;
			CPPUNIT_ASSERT(mean - tolerance <= sum);
			CPPUNIT_ASSERT(mean + tolerance >= sum);
		}

		void testOmnetVersion() {
			RngProvider::getInstance().setUseDefaultRngs(false);
			tester = new TesterClass();
			CPPUNIT_ASSERT_THROW(tester->getRandomInt(0, 1), std::runtime_error);
			CPPUNIT_ASSERT_EQUAL(0, ((OmnetIntegerUniformRng*) RngProvider::getInstance().int_rng_callers[tester])->k);
			TesterClass tester2 = TesterClass();
			CPPUNIT_ASSERT_EQUAL(1, ((OmnetIntegerUniformRng*) RngProvider::getInstance().int_rng_callers[&tester2])->k);
		}

	CPPUNIT_TEST_SUITE(RngProviderTests);
			CPPUNIT_TEST(testGetInts);
			CPPUNIT_TEST(testOmnetVersion);
		CPPUNIT_TEST_SUITE_END();
	};
}