cmake_minimum_required(VERSION 2.6.0)

project(EurobotIA CXX)

add_subdirectory("api")

include_directories("api")

add_library(demo SHARED Demo.cxx)

set_target_properties(demo PROPERTIES COMPILE_FLAGS -std=gnu++0x)
