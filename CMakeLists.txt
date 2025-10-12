cmake_minimum_required(VERSION 4.1.1)

project(FABRIKPD2D VERSION 1.0)

add_executable(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_sources(test PRIVATE
    src/fabrik.cpp
    test/test.cpp
)

target_include_directories(test PRIVATE src include)

target_link_directories(test PRIVATE lib)
target_link_libraries(test PRIVATE raylib user32 opengl32 kernel32 gdi32)