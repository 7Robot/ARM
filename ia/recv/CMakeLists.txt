cmake_minimum_required(VERSION 2.6.0)

file(GLOB SRC *.cxx)
set_source_files_properties(${SRC} PROPERTIES
	COMPILE_FLAGS "-Wno-write-strings -std=gnu++0x")

add_library(recv SHARED Asserv.cxx)
