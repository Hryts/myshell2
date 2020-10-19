cmake_minimum_required(VERSION 3.16)
project(mycat)

set(CMAKE_CXX_STANDARD 17)

add_executable(mycat src/main.cpp mycat/mycat.cpp mycat/mycat.h)
set(EXECUTABLE_OUTPUT_PATH ../utils)
