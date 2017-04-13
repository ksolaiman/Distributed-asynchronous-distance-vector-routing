cmake_minimum_required(VERSION 3.6)
project(DVR)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER /usr/local/bin/gcc-6)
set(SOURCE_FILES prog3.c node0.c node1.c node2.c node3.c)
add_executable(DVR ${SOURCE_FILES})