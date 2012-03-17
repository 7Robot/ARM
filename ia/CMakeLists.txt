cmake_minimum_required(VERSION 2.6.0)

project(EurobotIA CXX)

add_subdirectory("robot")
include_directories("robot")

#add_subdirectory("send")
#include_directories("send")

#add_subdirectory("recv")
#include_directories("recv")

add_subdirectory("module")
include_directories("module")

file(GLOB SRC *.cxx)
set_source_files_properties(${SRC} PROPERTIES
	COMPILE_FLAGS "-Wno-write-strings -std=gnu++0x")

add_executable(main main)

target_link_libraries(main pthread dl functions robot send recv)
