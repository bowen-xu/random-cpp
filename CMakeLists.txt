cmake_minimum_required(VERSION 2.8.4)
project(random_cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

set(SOURCE_FILES
        src/random.cpp)
add_library(random-cpp SHARED
        ${SOURCE_FILES})