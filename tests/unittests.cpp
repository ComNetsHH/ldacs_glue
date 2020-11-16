#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include "SequenceNumberTests.cpp"
#include "L2HeaderTests.cpp"

using namespace std;

int main(int argc, const char* argv[]) {
    CppUnit::TestResult result;
    CppUnit::TestResultCollector collectedResults;
    CppUnit::BriefTestProgressListener progress;
    CppUnit::TextUi::TestRunner runner;

    result.addListener (&collectedResults);
    result.addListener (&progress);

    runner.addTest(SequenceNumberTests::suite());
	runner.addTest(L2HeaderTests::suite());

    runner.run(result);
    return collectedResults.wasSuccessful() ? 0 : 1;

}
