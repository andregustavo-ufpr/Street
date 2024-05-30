cmake_minimum_required(VERSION 3.28)
project(Street C)

set(CMAKE_C_STANDARD 99)

add_executable(Street main.c)

target_link_libraries(Street -lmingw32 -lallegro.dll)