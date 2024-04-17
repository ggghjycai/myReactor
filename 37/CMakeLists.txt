cmake_minimum_required(VERSION 3.20.0)
project(SERVER CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/server/include
                    ${PROJECT_SOURCE_DIR}/affairs/include
                    ${PROJECT_SOURCE_DIR}/thread/include
                    ${PROJECT_SOURCE_DIR}/timer/include
                    ${PROJECT_SOURCE_DIR}/cas/include
                   )


file(GLOB SERVER ${PROJECT_SOURCE_DIR}/server/src/*.cpp)
file(GLOB EchoServer ${PROJECT_SOURCE_DIR}/affairs/code/*.cpp)
file(GLOB THREAD ${PROJECT_SOURCE_DIR}/thread/src/*.cpp)
file(GLOB TIMER ${PROJECT_SOURCE_DIR}/timer/src/*.cpp)
file(GLOB CAS ${PROJECT_SOURCE_DIR}/cas/src/*.cpp)


add_compile_options(-g  -lpthread)

add_executable(echoServer echoserver.cpp ${SERVER} ${EchoServer} ${THREAD} ${TIMER} ${CAS})
