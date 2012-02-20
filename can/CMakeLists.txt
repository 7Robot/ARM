cmake_minimum_required(VERSION 2.6.0)

add_subdirectory("libcan")

include_directories("libcan")

add_executable(canemit canemit.c)
add_executable(canconvert canconvert.c)
add_executable(canbridge canbridge.c)

target_link_libraries(canemit can)
target_link_libraries(canconvert can)
target_link_libraries(canbridge can functions)

set_target_properties(canemit canconvert canbridge PROPERTIES COMPILE_FLAGS -std=gnu99)

install(TARGETS canemit canconvert canbridge RUNTIME DESTINATION bin)
