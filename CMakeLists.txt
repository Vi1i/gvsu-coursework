cmake_minimum_required(VERSION 3.6)
project(miSim)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(MySQL++ REQUIRED)
find_package(MySQL REQUIRED)

include_directories(${MYSQL++_INCLUDE_DIR} ${MYSQL_INCLUDE_DIR} REQUIRED)

set(SOURCE_FILES main.cpp headers/Person.h src/Person.cpp headers/Tuberculosis.h headers/Sim.h src/Sim.cpp headers/DBConnetionManager.h src/Tuberculosis.cpp src/DBConnectionManager.cpp)
add_executable(miSim ${SOURCE_FILES})

target_link_libraries(miSim ${MYSQL++_LIBRARIES} ${MYSQL_LIBRARY})