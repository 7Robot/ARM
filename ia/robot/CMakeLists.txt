cmake_minimum_required(VERSION 2.6.0)

file(GLOB SRC *.cxx)
set_source_files_properties(${SRC} PROPERTIES
	COMPILE_FLAGS "-Wno-write-strings -std=gnu++0x")

add_subdirectory("queue")

link_libraries(robot queue)

add_library(robot STATIC Can Module ModuleManager Robot)

add_subdirectory("recv")
add_subdirectory("send")
include_directories("recv" "send")
