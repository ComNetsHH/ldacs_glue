cmake_minimum_required(VERSION 3.13)
project(intairnet_linklayer_glue)

set(CMAKE_CXX_STANDARD 14)

# Remove if not needed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include -I/usr/local/include/ -L/usr/local/Cellar/cppunit/1.15.1/lib")


add_library(intairnet_linklayer_glue SHARED library.cpp library.h L2Header.hpp MacId.hpp L2Packet.hpp CPRPosition.hpp IMac.hpp IArq.hpp IRlc.hpp SequenceNumber.hpp SequenceNumber.cpp)

add_executable(unittests tests/unittests.cpp tests/SequenceNumberTests.cpp)
target_link_libraries(unittests cppunit intairnet_linklayer_glue)
#target_link_libraries(unittests :libcppunit.so tuhh_intairnet_arq)