cmake_minimum_required(VERSION 2.6.0)

project(EurobotIA CXX)

add_subdirectory("queue")
include_directories("queue")

add_subdirectory("can")
include_directories("can")

add_subdirectory("send")
#include_directories("send")

add_subdirectory("recv")
#include_directories("recv")

add_subdirectory("module")
include_directories("module")

file(GLOB SRC *.cxx)
set_source_files_properties(${SRC} PROPERTIES
	COMPILE_FLAGS "-Wno-write-strings -std=gnu++0x")

add_executable(main main)

target_link_libraries(main functions pthread canpp queue send module recv)
