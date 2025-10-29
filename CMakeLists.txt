# https://gitlab.com/CLIUtils/modern-cmake

cmake_minimum_required(VERSION 3.12)

option(HOUSE_KEEPING_MODE "All Warnings, All Warnings as Errors, Clang Compilation" OFF)

# Set compiler to Clang before project() call
if (HOUSE_KEEPING_MODE)
    find_program(CLANG_C_COMPILER clang)
    find_program(CLANG_CXX_COMPILER clang++)

    if(CLANG_C_COMPILER AND CLANG_CXX_COMPILER)

        set(CMAKE_C_COMPILER ${CLANG_C_COMPILER})
        set(CMAKE_CXX_COMPILER ${CLANG_CXX_COMPILER})

        message(STATUS "UT HOUSE KEEPING MODE ENABLED WITH CLANG")
    else()
        message(WARNING "UT HOUSE KEEPING MODE DISABLED: CLANG NOT FOUND")
        set(HOUSE_KEEPING_MODE OFF)
    endif()
else()
    message(STATUS "UT HOUSE KEEPING MODE DISABLED")
endif()

project(
    ut
    VERSION "25.10.29"
    DESCRIPTION "ut, my personal c++ code library"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure headers
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ut/ut.hpp.in
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ut/ut.hpp
        @ONLY)

# Library source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Create the library
add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Housekeeping Compiler Flags
if (HOUSE_KEEPING_MODE)
target_compile_options(${PROJECT_NAME} PRIVATE
        # Turn on all warnings, make all warnings into errors
        -Wall -Wextra -Wpedantic -Werror

        # Warnings that I intentionally want to ignore
        -Wno-c++98-compat           # I don't care about legacy compat
        -Wno-c++98-compat-pedantic  # ditto
        -Wno-gnu-anonymous-struct   # Non standard but well defined, makes things like vector math a lot easier
        -Wno-nested-anon-types      # ditto
        -Wno-unused-function        # Doesn't make sense to enforce this in a library.

        # Warnings that I want to eventually enforce
        -Wno-unused-parameter
        -Wno-sign-compare

)
else()

endif()

# Testing target (separate from CMake's testing features)
#option(BUILD_TESTS "Build the test program" OFF)
#
if(BUILD_TESTS)
    # Test source files
    file(GLOB_RECURSE TEST_SOURCES "testing/*.cpp")

    # Create testing executable
    add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})

    # Link against our library
    target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})

    # Include testing directories
    target_include_directories(${PROJECT_NAME}_test
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/tests
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
endif()