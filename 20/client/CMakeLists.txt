cmake_minimum_required(VERSION 3.20.0)
project(CLIENT CXX)

#设置C++的标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(client client.cpp)
