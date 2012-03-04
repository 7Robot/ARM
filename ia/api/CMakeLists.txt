cmake_minimum_required(VERSION 2.6.0)

add_subdirectory(tasks)
include_directories(tasks)

add_executable(ia main Can Mission MissionHandler Callback Queue Callback)

set_target_properties(ia PROPERTIES COMPILE_FLAGS -std=gnu++0x)

file(GLOB SRC *.cxx)
set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS -Wno-write-strings)

target_link_libraries(ia dl functions can tasks)
