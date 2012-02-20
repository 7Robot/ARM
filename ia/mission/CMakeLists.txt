cmake_minimum_required(VERSION 2.6.0)

project(PetitRobot CXX)

include_directories(".." "../callback")

add_library(mission STATIC Mission Demo Lingo)
