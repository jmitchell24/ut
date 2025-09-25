# https://gitlab.com/CLIUtils/modern-cmake

cmake_minimum_required(VERSION 3.12)

project(
    ut
    VERSION "25.9.25"
    DESCRIPTION "ut, my personal c++ code library"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wformat-security")
# set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

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

# Testing target (separate from CMake's testing features)
option(BUILD_TESTS "Build the test program" OFF)

if(BUILD_TESTS)
    # Test source files
    file(GLOB_RECURSE TEST_SOURCES "testing/*.cpp")

    # Create testing executable
    add_executable(${PROJECT_NAME}_test ${TEST_SOURCES}
            testing/2025-04-18_main.cpp.old)

    # Link against our library
    target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})

    # Include testing directories
    target_include_directories(${PROJECT_NAME}_test
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/tests
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
endif()