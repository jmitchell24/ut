file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${UT_SOURCE_DIR}/include/ut/*.hpp")

# Make an automatic library - will be static or dynamic based on user setting
add_library(ut
    "Fmt.cpp"
    "Vector.cpp"
    "Color.cpp"
    ${HEADER_LIST}
)

# We need this directory, and users of our library will need it too
target_include_directories(ut PUBLIC ../include)

# All users of this library will need at least C++17
target_compile_features(ut PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
source_group(
    TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST}
)
