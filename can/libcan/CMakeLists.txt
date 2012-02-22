cmake_minimum_required(VERSION 2.6.0)

project(LibCan C)

file (GLOB SRC_FILES *.c)

add_library(can SHARED ${SRC_FILES})

target_link_libraries(can pthread)

set_target_properties(can PROPERTIES COMPILE_FLAGS "-std=gnu99 -fPIC")

install(TARGETS can LIBRARY DESTINATION lib)
install(FILES libcan.h DESTINATION include)
install(FILES can-messages.txt DESTINATION /etc/canconvert/)
