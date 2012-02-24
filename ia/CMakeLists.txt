cmake_minimum_required(VERSION 2.6.0)

project(EurobotIA CXX)

add_subdirectory("api")

include_directories("api")

add_library(template SHARED Template.cxx)
add_library(init SHARED Init.cxx)
add_library(petit SHARED Petit.cxx)
