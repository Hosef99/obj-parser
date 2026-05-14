cmake_minimum_required(VERSION 3.16)
project(ObjParser)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC
    src/*.cpp
)

add_executable(obj-parser ${SRC})

target_include_directories(obj-parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)
