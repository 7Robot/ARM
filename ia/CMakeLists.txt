cmake_minimum_required(VERSION 2.6.0)

project(EurobotIA CXX)

file(GLOB SRC *.cxx)

add_subdirectory("api")

include_directories("api")

set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS -Wno-write-strings)

add_library(template SHARED Template.cxx)
add_library(init SHARED Init.cxx)
add_library(petit SHARED Petit.cxx)
add_library(evitement SHARED Evitement.cxx)
