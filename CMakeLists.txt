cmake_minimum_required(VERSION 3.28)
project(Street C)

set(CMAKE_C_STANDARD 99)

add_executable(Street main.c)

INCLUDE_DIRECTORIES(  /opt/homebrew/Cellar/allegro/5.2.9.1/include)
LINK_DIRECTORIES(  /opt/homebrew/Cellar/allegro/5.2.9.1/lib )

file(GLOB LIBRARIES "/opt/homebrew/Cellar/allegro/5.2.9.1/lib/*.dylib")
message("LIBRARIES = ${LIBRARIES}")

TARGET_LINK_LIBRARIES(Street  ${LIBRARIES})

#target_link_libraries(Street -lmingw32 -lallegro.dll)