cmake_minimum_required(VERSION 3.13)
project(intairnet_linklayer_glue)

set(CMAKE_CXX_STANDARD 14)
set(GLUE_SRC_HPP L2Header.hpp MacId.hpp L2Packet.hpp CPRPosition.hpp IMac.hpp IArq.hpp IRlc.hpp SequenceNumber.hpp IPhy.hpp INet.hpp Timestamp.hpp)
set(GLUE_SRC_CPP SequenceNumber.cpp L2Packet.cpp IMac.cpp IArq.cpp)
set(GLUE_SRC_TESTS tests/unittests.cpp tests/SequenceNumberTests.cpp tests/L2HeaderTests.cpp tests/L2PacketTests.cpp)

# Remove if not needed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include -I/usr/local/include/ -L/usr/local/Cellar/cppunit/1.15.1/lib")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(intairnet_linklayer_glue SHARED ${GLUE_SRC_HPP} ${GLUE_SRC_CPP})
add_executable(glue-lib-unittests ${GLUE_SRC_HPP} ${GLUE_SRC_TESTS})
target_link_libraries(glue-lib-unittests cppunit intairnet_linklayer_glue)