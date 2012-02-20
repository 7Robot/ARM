cmake_minimum_required(VERSION 2.6.0)

project(PetitRobot CXX)

#file(GLOB SRC_FILES src/*.cc)

add_subdirectory("callback")
add_subdirectory("mission")

include_directories("callback" "mission")

add_executable(ia main.cc Robot.cc CanHandler.cc)

target_link_libraries(ia functions callback mission can)

#set_target_properties(ia PROPERTIES LINK_FLAGS -lcan)

#install(TARGETS ia RUNTIME DESTINATION bin)
