cmake_minimum_required(VERSION 3.20)
project(ipv4_subnet_calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(enable_project_warnings target_name)
    target_compile_options(${target_name} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall;-Wextra;-Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
    )
endfunction()

add_executable(ipv4_subnet_calculator
    src/main.cpp
    src/subnet.cpp
    include/subnet.h
)

target_include_directories(ipv4_subnet_calculator PRIVATE include)
enable_project_warnings(ipv4_subnet_calculator)

include(CTest)

if(BUILD_TESTING)
    add_executable(ipv4_subnet_tests
        tests/subnet_test.cpp
        src/subnet.cpp
        include/subnet.h
    )

    target_include_directories(ipv4_subnet_tests PRIVATE include)
    enable_project_warnings(ipv4_subnet_tests)

    add_test(
        NAME cpp_contract_tests
        COMMAND ipv4_subnet_tests "${CMAKE_CURRENT_SOURCE_DIR}/../tests/cases.tsv"
    )
endif()
