cmake_minimum_required(VERSION 2.6.0)

project(Joystick C)

add_executable(jsrobot jsrobot.c)

target_link_libraries(jsrobot pthread can functions)

install(TARGETS jsrobot RUNTIME DESTINATION bin)
