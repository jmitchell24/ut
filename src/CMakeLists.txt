file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/ut/*.hpp")

# Make an automatic library - will be static or dynamic based on user setting
add_library(ut
        "math.cpp"
        "color.cpp"
        ${HEADER_LIST}
)

# We need this directory, and users of our library will need it too
target_include_directories(ut PUBLIC ../include)

# All users of this library will need at least C++17
target_compile_features(ut PUBLIC cxx_std_17)




#
# The "Headers" source group for IDEs
#
list(FILTER HEADER_LIST INCLUDE REGEX .hpp$)
list(JOIN HEADER_LIST " " EXECUTABLE_HEADERS)
source_group("Header Files" FILES "${HEADER_LIST}")
