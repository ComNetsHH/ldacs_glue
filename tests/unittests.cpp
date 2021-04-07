//#include <cppunit/TestResult.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResultCollector.h>
//#include <cppunit/BriefTestProgressListener.h>
//#include <cppunit/CompilerOutputter.h>
#include "SequenceNumberTests.cpp"
#include "L2HeaderTests.cpp"
#include "L2PacketTests.cpp"

using namespace std;

int main(int argc, const char* argv[]) {
//    CppUnit::TestResult result;
//    CppUnit::TestResultCollector collectedResults;
//    CppUnit::BriefTestProgressListener progress;
	CppUnit::TextUi::TestRunner runner;

//    result.addListener (&collectedResults);
//    result.addListener (&progress);

	runner.addTest(SequenceNumberTests::suite());
	runner.addTest(L2HeaderTests::suite());
	runner.addTest(L2PacketTests::suite());

//    runner.run(result);
	runner.run();
	return runner.result().wasSuccessful() ? 0 : 1;
//    return collectedResults.wasSuccessful() ? 0 : 1;

}
