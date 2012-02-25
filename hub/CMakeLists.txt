cmake_minimum_required(VERSION 2.6.0)

project(TcpHub C)

add_executable(tcphub tcphub.c server.c)
add_executable(tcpmhd tcpmhd.c)

#target_link_libraries(tcphub rt)

set_target_properties(tcphub PROPERTIES COMPILE_FLAGS -std=gnu99)

install(TARGETS tcphub RUNTIME DESTINATION bin)
