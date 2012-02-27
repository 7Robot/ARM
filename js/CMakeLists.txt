cmake_minimum_required(VERSION 2.6.0)

project(Joystick C)

#add_executable(jstest jstest.c)
add_executable(jsrobot jsrobot.c)

target_link_libraries(jsrobot pthread can functions)

#set_target_properties(jstest PROPERTIES COMPILE_FLAGS -std=gnu99)

#install(TARGETS jstest RUNTIME DESTINATION bin)
