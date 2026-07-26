cmake_minimum_required(VERSION 3.20)
project(ipv4_subnet_calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ipv4_subnet_calculator
    src/main.cpp
    src/subnet.cpp
    include/subnet.h
)

target_include_directories(ipv4_subnet_calculator PRIVATE include)

target_compile_options(ipv4_subnet_calculator PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall;-Wextra;-Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
