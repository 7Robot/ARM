cmake_minimum_required(VERSION 2.6.0)

add_executable(fb2rgb fb2rgb.c)

set_target_properties(fb2rgb PROPERTIES COMPILE_FLAGS -std=gnu99)

install(TARGETS fb2rgb RUNTIME DESTINATION bin)
