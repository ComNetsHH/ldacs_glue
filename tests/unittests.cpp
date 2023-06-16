// The L-Band Digital Aeronautical Communications System (LDACS) Link Layer Glue Library provides interfaces and common classes necessary for the LDACS Air-Air Medium Access Control simulator.
// Copyright (C) 2023  Sebastian Lindner, Konrad Fuger, Musab Ahmed Eltayeb Ahmed, Andreas Timm-Giel, Institute of Communication Networks, Hamburg University of Technology, Hamburg, Germany
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

//#include <cppunit/TestResult.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResultCollector.h>
//#include <cppunit/BriefTestProgressListener.h>
//#include <cppunit/CompilerOutputter.h>
#include "SequenceNumberTests.cpp"
#include "L2HeaderTests.cpp"
#include "L2PacketTests.cpp"
#include "RngProviderTests.cpp"

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
	runner.addTest(RngProviderTests::suite());

//    runner.run(result);
	runner.run();
	return runner.result().wasSuccessful() ? 0 : 1;
//    return collectedResults.wasSuccessful() ? 0 : 1;

}
