# Testing library

# Adds Catch2::Catch2
# FetchContent_Declare(
#   catch
#   GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#   GIT_TAG v2.13.6)
# FetchContent_MakeAvailable(catch)


# fmt
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 8.0.1)
FetchContent_MakeAvailable(fmt)

#FetchContent_GetProperties(ftxui)
#if(NOT ftxui_POPULATED)
#  FetchContent_Populate(ftxui)
#  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
#endif()

# Tests need to be added as executables first
add_executable(ut_test ut_test.cpp)

if (MINGW)
    add_definitions("-Wa,-mbig-obj")
endif()

# I'm using C++17 in the test
target_compile_features(ut_test PRIVATE cxx_std_17)

# Should be linked to the main library, as well as the Catch2 testing library
# target_link_libraries(ut_test PRIVATE ut Catch2::Catch2)
target_link_libraries(ut_test PRIVATE ut fmt)

# If you register a test, then ctest and make test will run it.
# You can also run examples and check the output, as well.
add_test(NAME test_ut_test COMMAND ut_test) # Command can be a target
