cmake_minimum_required(VERSION 3.20.0)
project(SERVERCXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(tcpepoll tcpepoll.cpp)
