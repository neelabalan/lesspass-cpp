cmake_minimum_required(VERSION 2.8.12)
project(lesspass-cpp)

add_definitions("-std=c++17")


# include(build/conanbuildinfo.cmake)
# conan_basic_setup()
include_directories(inc)
add_library(library INTERFACE)
target_include_directories(library INTERFACE inc/)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra -pedantic-errors")


add_executable(lesspass-cpp src/main.cpp)

set_target_properties(lesspass-cpp PROPERTIES LINKER_LANGUAGE CXX)
# target_link_libraries(lesspass ${CONAN_LIBS})
target_link_libraries(lesspass-cpp ssl crypto)
target_link_libraries(lesspass-cpp library)