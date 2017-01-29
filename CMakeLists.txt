cmake_minimum_required(VERSION 2.8.4)
project(random_cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)
include_directories(opt/Catch/include)

set(SOURCE_FILES
        src/random.cpp)
add_library(randomcpp SHARED
        ${SOURCE_FILES})

set(TEST_FILES
        test/main.cpp
        test/random.cpp)
add_executable(randomcpp_test 
        ${TEST_FILES})
target_link_libraries(randomcpp_test randomcpp)

add_test(NAME randomcpp_test  COMMAND geneticpp-test)