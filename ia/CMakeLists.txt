cmake_minimum_required(VERSION 2.6.0)

project(EurobotIA CXX)

file(GLOB SRC *.cxx)

add_subdirectory("api")
include_directories("api")

add_subdirectory("gui")

set_source_files_properties(${SRC} PROPERTIES
	COMPILE_FLAGS "-Wno-write-strings -std=gnu++0x")

add_library(template SHARED Template.cxx)
add_library(recalage SHARED Recalage.cxx)
add_library(petit SHARED Petit.cxx)
add_library(evitement SHARED Evitement.cxx)
add_library(fwd SHARED Fwd.cxx)
add_library(str SHARED Str.cxx)
add_library(simulateur SHARED Simulateur.cxx)
