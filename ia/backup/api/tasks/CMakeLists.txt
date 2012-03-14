cmake_minimum_required(VERSION 2.6.0)

add_library(tasks STATIC Task.cxx TaskCmp.cxx
	TaskLoad.cxx TaskUnload.cxx
	TaskCanSend.cxx TaskCanRecv.cxx
	TaskSleep.cxx)

set_target_properties(tasks PROPERTIES COMPILE_FLAGS -std=gnu++0x)

file(GLOB SRC *.cxx)
set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS -Wno-write-strings)
