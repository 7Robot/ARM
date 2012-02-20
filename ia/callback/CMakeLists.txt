cmake_minimum_required(VERSION 2.6.0)

project(PetitRobot CXX)

add_library(callback STATIC CallbackSet CallbackHandler)
