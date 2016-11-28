cmake_minimum_required(VERSION 3.6)
project(miSim)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp headers/person.h src/person.cpp headers/tuberculosis.h headers/school.h)
add_executable(miSim ${SOURCE_FILES})