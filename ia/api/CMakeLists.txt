cmake_minimum_required(VERSION 2.6.0)

add_executable(ia main.cxx Can.cxx Mission.cxx MissionHandler.cxx)

target_link_libraries(ia dl functions can)
