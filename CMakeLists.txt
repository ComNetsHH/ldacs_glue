cmake_minimum_required(VERSION 3.1)
project(intairnet_linklayer_glue)

set(CMAKE_CXX_STANDARD 14)

set(GLUE_SRC_HPP L2Header.hpp MacId.hpp L2Packet.hpp CPRPosition.hpp IMac.hpp IArq.hpp IRlc.hpp SequenceNumber.hpp IPhy.hpp INet.hpp Timestamp.hpp IRadio.hpp IOmnetPluggable.hpp PassThroughRlc.hpp L3Packet.hpp PassThroughArq.hpp DelayMac.hpp InetPacketPayload.hpp RngProvider.hpp Statistic.hpp ContentionMethod.hpp LinkProposal.hpp SlotDuration.hpp DutyCycleBudgetStrategy.hpp)
set(GLUE_SRC_CPP SequenceNumber.cpp L2Packet.cpp IMac.cpp IArq.cpp IPhy.cpp IRadio.cpp PassThroughRlc.cpp L3Packet.cpp PassThroughArq.cpp DelayMac.cpp InetPacketPayload.cpp RngProvider.cpp Statistic.cpp SimulatorPosition.hpp)

set(GLUE_SRC_TESTS tests/unittests.cpp tests/SequenceNumberTests.cpp tests/L2HeaderTests.cpp tests/L2PacketTests.cpp RngProvider.cpp RngProvider.hpp tests/RngProviderTests.cpp)

# Remove if not needed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include/ -L/usr/local/Cellar/cppunit/1.15.1/lib")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(intairnet_linklayer_glue SHARED ${GLUE_SRC_HPP} ${GLUE_SRC_CPP})
add_executable(glue-lib-unittests ${GLUE_SRC_HPP} ${GLUE_SRC_TESTS})
target_include_directories(glue-lib-unittests PUBLIC /opt/homebrew/opt/cppunit/include)
find_library(CPPUNITLIB cppunit)
target_link_libraries(glue-lib-unittests ${CPPUNITLIB} intairnet_linklayer_glue)
