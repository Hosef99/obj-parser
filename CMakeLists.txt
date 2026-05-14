cmake_minimum_required(VERSION 3.16)
project(ObjParser)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC
    src/*.cpp
)

add_library(obj-parser STATIC)

target_sources(obj-parser PRIVATE
    ${SRC}
)

target_include_directories(obj-parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
