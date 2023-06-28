# Testing library

# FetchContent added in CMake 3.11, downloads during the configure step
include(FetchContent)

#
# fmt
#

#FetchContent_Declare(
#    fmt
#    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
#    GIT_TAG 8.0.1)
#FetchContent_MakeAvailable(fmt)

set(TEST_EXECUTABLE "${PROJECT_NAME}-test")

add_executable(${TEST_EXECUTABLE})

file(GLOB_RECURSE TEST_SOURCES
        "*.c"
        "*.h"
        "*.cpp"
        "*.hpp"
        )

target_sources(${TEST_EXECUTABLE} PRIVATE ${TEST_SOURCES})

target_include_directories(${TEST_EXECUTABLE}
        PRIVATE
        test
        )




if (MINGW)
    add_definitions("-Wa,-mbig-obj")
endif()

# I'm using C++17 in the test
target_compile_features(${TEST_EXECUTABLE} PRIVATE cxx_std_17)

# Should be linked to the main library, as well as the Catch2 testing library
# target_link_libraries(ut_test PRIVATE ut Catch2::Catch2)
target_link_libraries(${TEST_EXECUTABLE} PRIVATE ut)
# target_link_libraries(${TEST_EXECUTABLE} PRIVATE ut fmt)

# If you register a test, then ctest and make test will run it.
# You can also run examples and check the output, as well.
add_test(NAME test_ut_test COMMAND ${TEST_EXECUTABLE}) # Command can be a target
