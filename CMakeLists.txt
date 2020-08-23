project(accumulatorplus)

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

include(cmake/accumula_config_generator.cmake)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.71.0 REQUIRED)
find_package(fmt REQUIRED)

include_directories(src)
link_libraries(
    Boost::boost
    fmt::fmt-header-only
)

file(GLOB INCLUDES "src/*.h" "src/*.hpp")
file(GLOB SOURCES "test/*.cpp")
add_executable(all_test 
    ${SOURCES}
    ${INCLUDES}
)

enable_testing()
add_test(all_test "${CMAKE_BINARY_DIR}/all_test")
