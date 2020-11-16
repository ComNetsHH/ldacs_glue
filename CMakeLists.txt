cmake_minimum_required(VERSION 3.13)
project(intairnet_linklayer_glue)

set(CMAKE_CXX_STANDARD 14)

add_library(intairnet_linklayer_glue SHARED library.cpp library.h L2Header.hpp MacId.hpp L2Packet.hpp CPRPosition.hpp IMac.hpp IArq.hpp IRlc.hpp)