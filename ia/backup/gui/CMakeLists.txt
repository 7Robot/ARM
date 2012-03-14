cmake_minimum_required(VERSION 2.6.0)

file(GLOB SRC *.cxx)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(".." "../ia")

set_source_files_properties(${SRC} PROPERTIES
	COMPILE_FLAGS "-Wno-write-strings -std=gnu++0x")

if(GTKMM_FOUND)
	include_directories(${GTKMM_INCLUDE_DIRS})
	add_library(gui SHARED
		Gui.cxx
		Map.cxx)
	target_link_libraries(gui ${GTKMM_LIBRARIES} pthread)
elseif(GTKMM_FOUND)
	message(SEND_ERROR "Gtkmm3 not found, can't build debugger !")
endif(GTKMM_FOUND)
