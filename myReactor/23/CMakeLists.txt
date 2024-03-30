cmake_minimum_required(VERSION 3.20.0)
project(SERVER CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/server/include ${PROJECT_SOURCE_DIR}/business/include)


file(GLOB SERVER ${PROJECT_SOURCE_DIR}/server/src/*.cpp)
file(GLOB EchoServer ${PROJECT_SOURCE_DIR}/business/code/*.cpp)


add_compile_options(-g)

add_executable(echoServer echoserver.cpp ${SERVER} ${EchoServer})
