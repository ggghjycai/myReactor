cmake_minimum_required(VERSION 3.20.0)
project(thread CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(threadPool test01.cpp ${SRC})

add_compile_options(-g -lpthread)