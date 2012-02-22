cmake_minimum_required(VERSION 2.6.0)

project(Eurobot)

add_subdirectory("functions")
include_directories("functions")

add_subdirectory("can")
include_directories("can/libcan")

add_subdirectory("gateway")

add_subdirectory("ia")

add_subdirectory("hub")

add_subdirectory("pictures/fb2rgb")
