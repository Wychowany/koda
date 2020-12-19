cmake_minimum_required(VERSION 3.13)
project(koda)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")

add_executable(koda main.cpp BAC.cpp BAC.h ImageReader.cpp ImageReader.h)