project(accumulatorplus)

cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.71.0 REQUIRED)
find_package(fmt REQUIRED)

include_directories(src)
link_libraries(
    Boost::boost
    fmt::fmt-header-only
)

file(GLOB INCLUDES "src/*.cpp" "src/*.h" "src/*.hpp")
add_executable(all_test 
	test/all.cpp
    ${INCLUDES} )

enable_testing()
add_test(all_test "${CMAKE_BINARY_DIR}/all_test")
